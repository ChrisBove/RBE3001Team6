/** @brief arm library
 *
 * @file arm.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 30-Jan-2016
 * @version 1.0
 */

#include "include/arm.h"
#include "RBELib/RBELib.h"
#include "include/definitions.h"
#include "math.h"

/**
 * @var degreesPerJoint1Val
 * for storing the degrees per adc value for joint 1
 *
 * @var degreesPerJoint2Val
 * for storing the degrees per adc value for joint 2
 *
 * @todo change these to constants
 */
float degreesPerJoint1Val;
float degreesPerJoint2Val;

/**
 * @var x_coord
 * for storing the current x coordinate of the arm
 *
 * @var y_coord
 * for storing the current y coordinate of the arm
 *
 * @todo get rid of these globals
 */
float x_coord;
float y_coord;

/**
 * @var L2L2
 * constant for optimizing Link 2 Length ^2
 * @var L3L3
 * constant for optimizing Link 3 Length ^2
 */
float L2L2;
float L3L3;

/**
 * @var lowerAngle
 * for storing the current lowerAngle of the arm
 *
 * @var upperAngle
 * for storing the current upperAngle of the arm
 *
 * @todo get rid of these globals
 */
int lowerAngle;
int upperAngle;

/**
 * @var servicePID
 * flag - TRUE if PID controller needs to be serviced, FALSE otherwise
 *
 * @var timerCount
 * for keeping time. increments in 0.01 seconds
 *
 */
volatile BOOL servicePID;
volatile unsigned long timerCount;

/**
 * @brief Timer ISR that runs at 100Hz
 * @details set flags for servicing at fixed intervals.
 *
 * @param TIMER0_COMPA_vect Interrupt vector for timer0 vector on AVR
 *
 */
ISR(TIMER0_COMPA_vect) {
	servicePID = TRUE; // time to service PID!
	timerCount++; // increment our counter
}

/**
 * @brief initialize the arm variables
 */
void initArm() {
	// calculate conversions from adc values to degrees for each joint
	degreesPerJoint1Val = 90.0/(JOINT_1_VAL_AT_90 - JOINT_1_VAL_AT_0);
	degreesPerJoint2Val = 90.0/(JOINT_2_VAL_AT_90 - JOINT_2_VAL_AT_0);

	//calculate some squares up front
	L2L2 = pow(LINK_2_Length,2);
	L3L3 = pow(LINK_3_Length,2);

	// intialize devices and set constants for PID controllers
	initADC(ADC3D); // init ADC
	stopMotors();
	setConst(2,20,0.1,4); // joint 2 - Kp, Ki, Kd
	setConst(3,20,0.1,4); // joint 3 - Kp, Ki, Kd
	setupTimer();
	setJointAngles(0,90); // set desired joint angles to 0
}

/**
 * @brief sets a 100Hz timer up on Timer 0
 */
void setupTimer() {
	// setup registers for the timer
	TCCR0A |= BIT(WGM01);// set to CTC
	TCCR0A |= BIT(COM0A1); // Configure timer 1 for CTC mode

	TCCR0B |= BIT(CS02) | BIT(CS00); // prescale by 1024 = 18kHz
	OCR0A = 179; // divide by 180 -1  to get 100 Hz count

	timerCount = 0; // initialize timercount
	TIMSK0 |= (1 << OCIE0A); // Enable CTC interrupt
	sei(); // Enable global interrupts
}

/**
 * @brief runs functions critical to arm operation. Call as often as possible.
 */
void serviceArm(){
	// if servicePID flag has been set (i.e. runs at 100Hz)
	if(servicePID){
		gotoAngles(lowerAngle, upperAngle); // run PID loop called in gotoAngles

	}
}

/**
 * @brief gets the current, calibrated joint angle of the passed joint number
 * @param  joint 1 or 2 of the joint to get the angle for
 *
 * @return angle of joint in degrees (generally 0 to 180)
 */
float getJointAngle(int joint){
	float result;
	// calculate angle with linear interpolation
	if(joint == 1)
		result = ((float)getADC(JOINT_1_ADC)-JOINT_1_VAL_AT_0)*degreesPerJoint1Val;
	else
		result = ((float)getADC(JOINT_2_ADC)-JOINT_2_VAL_AT_0)*degreesPerJoint2Val;
	return result;
}

/**
 * @brief gets motor current of specified joint
 * @param  joint The joint to get the current for
 *
 * @return current in mA
 */
int getCurrent(int joint){
	unsigned short adc_val = -1; // leave -1 to check for valid numbers
	float milliamps = 0;
	// get adc_val based on channel for the joint
	if(joint == 1) 	{
		adc_val = getADC(ADC0D) - 49; // take off a zero offset
	}
	else if(joint == 2){
		adc_val = getADC(ADC1D) - 49; // take off a zero offset
	}

	// convert to milliamps if valid
	if(adc_val != -1){
		milliamps = 4.89 * adc_val - 2500;
	}
	return milliamps;

}
/**
 * @brief Function for managing the currents gathered and averaging them when asked
 * @param  current Current to be added to the stored values
 * @param  command Command of what to be done with the function, either resetCurrent,addCurrent,retrieveAverageCurrent
 *
 * @return current in mA
 */
float getAverageCurrent(int command,int current){//takes in a command and a current reading
	static float totalCurrent = 0;//initialize total current variable
	static int numberOfCurrents = 0;//initialize numberOfcurrents variable, its the number of readings taken
	static float averageCurrent = 0;//Initiaize a variable for the average current reading
	if(command == addCurrent){//adds current given to the total current readings and indexes the number of readings taken
		totalCurrent += current;
		numberOfCurrents ++;
		return numberOfCurrents;
	}
	else if(command == resetCurrent){//resets all variables
		totalCurrent = 0;
		numberOfCurrents = 0;
		averageCurrent = 0;
		return numberOfCurrents;
	}
	else if(command == retrieveAverageCurrent){//Averages total current by diving by number of instances added
		averageCurrent = totalCurrent/numberOfCurrents;
		return averageCurrent;
	}
	else{
		printf("You don messed up");
		return -1;
	}
}

/**
 * @brief updates globals with new desired ones
 * @param  lowerJoint position for the lower joint 1
 * @param  upperJoint position for the upper joint 2
 *
 */
void setJointAngles(int lowerJoint, int upperJoint){
	lowerAngle = lowerJoint;
	upperAngle = upperJoint;
}

/**
 * @brief gets the time in seconds
 *
 * @return time in seconds
 */
float getTimeSeconds(){
	return timerCount/100.0;
}

/**
 * @brief calculates forward kinematics for arm and updates global position
 */
void calcXY(){
	// calc joint angles
	float Joint1Angle = getJointAngle(1)*RADS_PER_DEGREE;
	float Joint2Angle = getJointAngle(2)*RADS_PER_DEGREE;

	// forward kinematics to calculate coordinates of end of link 2
	float x1 = LINK_2_Length*cos(Joint1Angle);
	float y1 = LINK_1_Length+(LINK_2_Length*sin(Joint1Angle));

	// forward kinematics to calculate coordinates of end effector link 3
	x_coord = x1 + LINK_3_Length*cos(Joint2Angle -(3.14159/2.0)+ Joint1Angle);
	y_coord = y1 + LINK_3_Length*sin(Joint2Angle-(3.14159/2.0)+ Joint1Angle);

}

/**
 * @brief check if the arm is in the angle position specified
 * @param  theta1 angle of first joint
 * @param  theta2 angle of second joint
 *
 * @return TRUE if arm is between
 * @todo pass in the threshold to use
 */
BOOL inPosition(int theta1, int theta2){
	// return if both joints are within a tolerance of +-2
	return( (betweenTwoVals(theta1-getJointAngle(1),-2,2))
			&& (betweenTwoVals(theta2-getJointAngle(2),-2,2)) );
}
/**
 * @brief checks if the arm is in desired position
 *
 * @return true if in desired position
 */
BOOL doneMoving(){
	return inPosition(lowerAngle,upperAngle);
}

/**
 * @brief calculates IK values and sets angles
 * @param x desired x position
 * @param y desired y position
 */
void setPosition(float x, float y){
	// subtract Link1 (vertical) length from requested y value
	float _y = y - LINK_1_Length;
	// optimization - cache these calculations
	float xx = pow(x,2);
	float yy = pow(_y,2);

	float theta1 = atan2f(_y,x)+acos((xx+yy+L2L2-L3L3)/(2*LINK_2_Length*(sqrt((xx+yy)))));
	float theta2 = acos(((L2L2)+(L3L3)-(xx+yy))/(2*LINK_2_Length*LINK_3_Length))-(3.14159/2);
	lowerAngle = theta1*DEGREES_TO_RADIANS;//sets results to the global variable lowerAngle
	upperAngle = theta2*DEGREES_TO_RADIANS;//sets results to the global variable upperAngle
}

/**
 * @brief uses a polynomial to calibrate the IR distance readings
 * @param IRDist distance reading in mm
 *
 * @return calibrated IR distance in mm
 */
int calibratedIRVal(int IRDist){
	return (-0.0081*pow(IRDist,2)) + 3.3029*IRDist - 171.62;
}
