/**
 * @file motors.c
 *
 * @brief Motor driving functions for the arm.
 *
 * @author Chris Bove
 * @date Feb 3, 2016
 */

#include "RBELib/RBELib.h"
#include "include/definitions.h"
#include "include/arm.h"

/**
 * @brief Helper function to stop the motors on the arm.
 */
void stopMotors(){
	// todo the command and address could be changed to quickly stop them all...
	setDAC(0,0);
	setDAC(1,0);
	setDAC(2,0);
	setDAC(3,0);
}

/**
 * @brief Drive the arm to a desired angle
 *
 * @param lowerTheta The desired angle for the lower link.
 * @param upperTheta The desired angle for the upper link.
 *
 */
void gotoAngles(int lowerTheta, int upperTheta){
	// compute PID control values and drive the link to those outputs
	lastJoint1Angle = getJointAngle(1); // for grav compensation on top link
	PID = calcPID(2,upperTheta,getJointAngle(2));
	driveLink(3,PID);
	int PID2 = calcPID(1,lowerTheta,getJointAngle(1));
	driveLink(2,PID2);
}

/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y){

}

/**
 * @brief Drive a link (upper or lower) in a desired direction.
 *
 * @param link Which link to control.
 * @param dir Which way to drive the link. Between -2048 and +2048
 *
 */
void driveLink(int link, int dir){
	// based on link and direction, set 1 channel to 0 and drive other one up
	if(link == 2){
		if(dir < 0){
			setDAC(JOINT_1_DAC_0, 0);
			setDAC(JOINT_1_DAC_1, 2*abs(dir));
		}
		else {
			setDAC(JOINT_1_DAC_0, 2*abs(dir));
			setDAC(JOINT_1_DAC_1, 0);
		}
	}
	else {
		if(dir < 0){
			setDAC(JOINT_2_DAC_0, 2 * abs(dir));
			setDAC(JOINT_2_DAC_1, 0);
		} else {
			setDAC(JOINT_2_DAC_0, 0);
			setDAC(JOINT_2_DAC_1, 2 * abs(dir));
		}
	}
}

/**
 * @brief Drive the arm to a "home" position using the potentiometers.  This
 * should be called before using the encoders and just goes to a default position.
 * Once this has been called once, you can initialize/clear the encoders.
 *
 */
void homePos(){
	// === move joint 1 ===
	// calculate direction to 0
	int dir = 0 - getJointAngle(2);
	// while not within +- 2 from 0, keep moving
	while(!betweenTwoVals(getJointAngle(2),-2,2)){
		// todo double check direction in case we blew past
		if(dir > 0)
			driveLink(3,300);
		else
			driveLink(3,-300);
	}
	driveLink(3,0); // stop

	// === move joint 2 ===
	dir = 0 - getJointAngle(1);
	// while not within +- 2 from 0, keep moving
	while(!betweenTwoVals(getJointAngle(1),-2,2)){
		if (dir > 0)
			driveLink(2, 300);
		else
			driveLink(2, -300);
	}
	driveLink(2,0); // stop
}
