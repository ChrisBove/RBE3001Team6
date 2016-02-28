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
#include "include/button.h"
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

	// intialize devices and set constants for PID controllers
	initADC(ADC3D); // init ADC
	stopMotors();
	setConst(2,20,0.1,4); // joint 2 - Kp, Ki, Kd
	setConst(3,20,0.1,4); // joint 3 - Kp, Ki, Kd
	setupTimer();
	setJointAngles(0,0); // set desired joint angles to 0
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

		//controlPositionWithButtons(); // setpoint based on buttons

		// print logging data for Lab 3
//		printf("%.2f,%.1f,%ld,%.3f,%.3f,%.3f\n\r", getTimeSeconds(), //time
//				getJointAngle(1),encCount(1), // joint angle, encoder position
//				getGs(0),getGs(1),getGs(2)); // x,y,z accelerometer G's

	}

}

/**
 * @brief prints the header for streaming joint angles
 */
void printHeaderForJointAngle(){
	printf("ADC Val,Pot Voltage(mV),Angle(deg)\n\r");
}

/**
 * @brief prints the header for the main logging of step responses
 */
void printHeaderForLogging(){
	printf("Time(s),Command Pos (degr),Orientation(degr),MotorInput(mV),"
			"Current(mA)\n\r");
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
 * @brief prints adcval,pot mV, and joint angle of the passed joint number
 * @param  joint 1 or 2 of the joint to print data for
 *
 */
void printJointAngle(int joint){
	if(joint == 1) {
		unsigned int adcval = getADC(JOINT_1_ADC);
		printf("%u,%.2f,%.2f\n\r", adcval,(5000.0 * adcval)/1023.0, getJointAngle(1));
	}
	else {
		unsigned int adcval = getADC(JOINT_2_ADC);
		printf("%u,%.2f,%.2f\n\r", adcval,(5000.0 * adcval)/1023.0, getJointAngle(2));
	}

}

/**
 * @brief prints time, setpoint, jointAngle, PID output, and motor current
 * @param setPoint the current command to the controller
 *
 */
void printLogLineJoint2(int setPoint) {
	printf("%.2f,%i,%.2f,%.2f,%i\n\r", getTimeSeconds(), setPoint,
			getJointAngle(2), PID/7.2, getCurrent(2));
	_delay_ms(20);
}

/**
 * @brief streams joint angles in degrees and (x,y) tip positions in mm
 */
void printJointAnglesAndPos(){
	calcXY(); // update the globals
	printf("%.2f,%.2f,%.1f,%.1f\n\r",getJointAngle(1),getJointAngle(2),x_coord,y_coord);
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
 * @brief converts a voltage value to a DAC value
 * @param  volts to convert to DAC -7.2 to 7.2 volts
 *
 * @return DAC value 0-4095
 */
int convertVoltsToDACVal(float volts){
	// 4096 counts/7.2V
	return abs(volts)*568.75;
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
 * @brief changes desired joint angle of arm based on button presses for testing
 * @note This does not run the PID control, just sets setpoint
 */
void controlArmWithButtons(){
	// based on last button pressed, set the joint angles
	switch(lastButtonPressed()){
	case 4:
		setJointAngles(0,0);
		break;
	case 5:
		setJointAngles(30,30);
		break;
	case 6:
		setJointAngles(45,45);
		break;
	case 7:
		setJointAngles(90,90);
		break;
	default:
		printf("odd button received\n\r");
		break;
	}
}

/**
 * @brief changes desired joint angles of arm based on button presses
 * @note This does not run the PID control, just sets setpoint
 */
void controlPositionWithButtons(){
	static unsigned char lastState = 0; // store last case
	// based on last button pressed, set the joint angles
	switch (lastButtonPressed()) {
	// for each case, if the state changed, print the state we're on
	// then set joint angles and update the last state variable
	case 4:
		if(lastState != 4)
			printf("0,90\n\r");
		setJointAngles(0, 90);
		lastState = 4;
		break;
	case 5:
		if(lastState != 5)
			printf("90, 90\n\r");
		setJointAngles(90, 90);
		lastState = 5;
		break;
	case 6:
		if(lastState != 6)
			printf("41,92\n\r");
		setJointAngles(41, 92);
		lastState = 6;
		break;
	case 7:
		if(lastState != 7)
			printf("149,-15\n\r");
		setJointAngles(149, -15);
		lastState = 7;
		break;
	default:
		printf("odd button received\n\r");
		break;
	}
}

/**
 * @brief Lab3 - control motor drive voltages with buttons
 * @details Sets first link to 0, -3, +3, or plus 6 Volts
 * button 4 sets voltage to 0
 * button 5 sets voltage to -3
 * button 6 sets voltage to +3
 * button 7 sets voltage to +6
 */
void controlJoint1WithButtons(){
	//switch output voltage based on last button pressed
	switch(lastButtonPressed()){
	// set voltage 0
	case 4:
		setDAC(JOINT_1_DAC_0,0);
		setDAC(JOINT_1_DAC_1,convertVoltsToDACVal(0));
		break;
	// set voltage -3
	case 5:
		setDAC(JOINT_1_DAC_0,convertVoltsToDACVal(3));
		setDAC(JOINT_1_DAC_1,0);
		break;
	// set voltage 3
	case 6:
		setDAC(JOINT_1_DAC_0,0);
		setDAC(JOINT_1_DAC_1,convertVoltsToDACVal(3));
		break;
	// set voltage 6
	case 7:
		setDAC(JOINT_1_DAC_0,0);
		setDAC(JOINT_1_DAC_1,convertVoltsToDACVal(6));
		break;

	default:

		break;
	}

}

/**
 * @brief state machine for drawing triangles.
 * @details button 4 draws a prescripted triangle
 * button 5 just moves to a pose
 * button 6 records a motion
 * button 7 plays back the motion
 * @note This does not run the PID control, just sets setpoint
 */
void drawTriangleWithButtons(){
	static unsigned char lastState = 0;

	static int point = 0; // point of triangle to track for scripted triangle
	static int pointArray[50][2]; // for storing recorded motion
	// todo make the array size not a magic number
	static unsigned long pointIndex = 0; // point index for the array
	static unsigned long drawingIndex = 0; // index for drawing points

	// switch based on the button pressed
	switch (lastButtonPressed()) {
	// draw scripted triangle
	case 4:
		// if new case, print that we are starting the draw
		if (lastState != 4){
			printf("Drawing Triangle\n\r");
			point = 0;
		}
		// switch based on what point of the triangle we are on
		switch(point){
		// each case changes the angles the arm goes in, case moves on when
		// the arm is in position
		// todo make these defined constants
		case 0:
			setJointAngles(73,-24);
			if(inPosition(73,-24))
				point = 1;
			break;
		case 1:
			setJointAngles(124, -13);
			if (inPosition(124, -13))
				point = 2;
			break;
		case 2:
			setJointAngles(56, 68);
			if (inPosition(56, 68))
				point = 3;
			break;
		case 3:
			setJointAngles(73, -24);
			if (inPosition(73, -24))
				point = 4;
			break;
		default:
			printf("Done triangle.\n\r");
			break;
		}
		lastState = 4;
		break;
	// go to scripted pose
	case 5:
		if (lastState != 5){
			printf("2, 181\n\r");
		}
		setJointAngles(2, 181);
		lastState = 5;
		break;
	// record points until array is filled
	case 6:
		if (lastState != 6){
			printf("record triangle\n\r");
			pointIndex = 0;
		}

		// stash points in the array until it fills
		if(pointIndex < 50){
			pointArray[pointIndex][0] = getJointAngle(1);
			pointArray[pointIndex][1] = getJointAngle(2);
			pointIndex++;
		}
		else {
			printf("done recording\n\r");
		}
		lastState = 6;

		break;
	// play back the stored points in the array
	case 7:
		if (lastState != 7){
			printf("draw points\n\r");
			drawingIndex = 0;
		}

		// keep setting the joints from the array until we hit the index
		if(drawingIndex<50){
			setJointAngles(pointArray[drawingIndex][0], pointArray[drawingIndex][1]);
			drawingIndex++;
		}
		lastState = 7;

		break;
	default:
		printf("odd button received\n\r");
		break;
	}
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
 * @brief updates 2 globals with the current xy in mm of the end effector
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
 * @brief runs getAccel and converts to G's
 * @param axis 0-2 for x,y,z axis to get g's on
 *
 * @return -3.0 to 3.0 g's of force
 */
float getGs(int axis){
	return getAccel(axis)*0.0022;
}

/**
 * @brief calculates IK values and sets angles
 * @param x desired x position
 * @param y desired y position
 */
void setPosition(double x, double y){
	float a1 = 15.24;//arm length 1 is 15.24 cm
	float a2 = 15.24;//arm length 2 to the center of the gripper is 15.24 cm
	float theta1 = atan2((y/x))+acos((x*x+y*y+a1*a1-a2*a2)/(2*a1*(sqrt((x*x+y*y)))));//not sure if we are dealing with degrees or radians , needs testing
	float theta2 = acos(((a1*a1)+(a2*a2)-(x*x+y*y))/(2*a1*a2))-(3.14159/2);//im not sure what acos will retun i terms of radians or degress so it may be to be modified
	lowerAngle = theta1;
	upperAngle = theta2;
}
