/** @brief arm library
 *
 * @file arm.h
 *
 *
 * @author cpbove@wpi.edu
 * @date 30-Jan-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"

#ifndef INCLUDE_ARM_H_
#define INCLUDE_ARM_H_

/**
 * @def JOINT_2_ADC
 * ADC channel for joint 2
 * @def JOINT_1_ADC
 * ADC channel for joint 1
 */
#define JOINT_2_ADC			ADC3D
#define JOINT_1_ADC			ADC2D
/**
 * @def JOINT_2_VAL_AT_0
 * ADC value of joint 2 at 0 degree position
 * @def JOINT_2_VAL_AT_90
 * ADC value of joint 2 at 90 degree position
 */
#define JOINT_2_VAL_AT_0 	280
#define JOINT_2_VAL_AT_90 	650
/**
 * @def JOINT_1_VAL_AT_0
 * ADC value of joint 1 at 0 degree position
 * @def JOINT_1_VAL_AT_90
 * ADC value of joint 1 at 90 degree position
 */
#define JOINT_1_VAL_AT_0 	180
#define JOINT_1_VAL_AT_90 	550

/**
 * @def LINK_1_Length
 * length of Link 1 in mm
 * @def LINK_2_Length
 * length of Link 2 in mm
 * @def LINK_3_Length
 * length of Link 3 in mm
 */
#define LINK_1_Length	144.10
#define LINK_2_Length	151.13
#define LINK_3_Length	154.75
/**
 * @enum currentCommands
 * For specifying commands for the current averaging function
 */
enum currentCommands {
	addCurrent,
	resetCurrent,
	retrieveAverageCurrent
};

/**
 * @brief initialize the arm variables
 */
void initArm();
/**
 * @brief sets a 100Hz timer up on Timer 0
 */
void setupTimer();
/**
 * @brief runs functions critical to arm operation. Call as often as possible.
 */
void serviceArm();
/**
 * @brief updates globals with new desired ones
 * @param  desired lowerJoint position for the lower joint 1
 * @param  desired upperJoint position for the upper joint 2
 *
 */
void setJointAngles(int lowerJoint, int upperJoint);
/**
 * @brief gets the current, calibrated joint angle of the passed joint number
 * @param  joint 1 or 2 of the joint to get the angle for
 *
 * @return angle of joint in degrees (generally 0 to 180)
 */
float getJointAngle(int joint);
/**
 * @brief allows adding to cumulative average, getting the average, and reseting
 * @param command using currentCommand enum for determining action to take
 *
 * @return average current value in mA
 */
float getAverageCurrent(int command, int current);
/**
 * @brief gets motor current of specified joint
 * @param  joint The joint to get the current for
 *
 * @return current in mA
 */
int getCurrent(int joint);
/**
 * @brief gets the time in seconds
 *
 * @return time in seconds
 */
float getTimeSeconds();
/**
 * @brief calculates forward kinematics for arm and updates global position
 */
void calcXY();
/**
 * @brief check if the arm is in the angle position specified
 * @param  theta1 angle of first joint
 * @param  theta2 angle of second joint
 *
 * @return TRUE if arm is between
 * @todo pass in the threshold to use
 */
BOOL inPosition(int theta1, int theta2);
/**
 * @brief checks if the arm is in desired position
 *
 * @return true if in desired position
 */
BOOL doneMoving();
/**
 * @brief calculates IK values and sets angles
 * @param x desired x position
 * @param y desired y position
 */
void setPosition(float x, float y);
/**
 * @brief uses a polynomial to calibrate the IR distance readings
 * @param IR distance reading in mm
 *
 * @return calibrated IR distance in mm
 */
int calibratedIRVal(int IRDist);

#endif /* INCLUDE_ARM_H_ */
