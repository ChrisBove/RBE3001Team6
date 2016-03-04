/** @brief Graveyard for old Lab 2 and 3 functions
 *
 * @file lab2and3.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 3-Mar-2016
 * @version 1.0
 */

#include "include/lab2and3.h"
#include "RBELib/RBELib.h"
#include "include/definitions.h"
#include "include/arm.h"
#include "include/button.h"

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
	//printf("%.2f,%.2f,%.1f,%.1f\n\r",getJointAngle(1),getJointAngle(2),x_coord,y_coord);
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
 * @brief runs getAccel and converts to G's
 * @param axis 0-2 for x,y,z axis to get g's on
 *
 * @return -3.0 to 3.0 g's of force
 */
float getGs(int axis){
	return getAccel(axis)*0.0022;
}
