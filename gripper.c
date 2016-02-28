/** @brief gripper library
 *
 * @file gripper.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 27-Feb-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/gripper.h"

/**
 * @brief Closes the gripper
 */
void closeGripper(){
	setServo(GRIPPER_PIN,GRIPPER_CLOSE);
}
/**
 * @brief Opens the gripper
 */
void openGripper(){
	setServo(GRIPPER_PIN,GRIPPER_OPEN);
}
/**
 * @brief starts the conveyor belt servo
 */
void startConveyor(){
	setServo(CONVEYOR_PIN, CONVEYOR_FORWARD);
}
/**
 * @brief stops the conveyor belt servo
 */
void stopConveyor(){
	setServo(CONVEYOR_PIN, CONVEYOR_STOP);
}
