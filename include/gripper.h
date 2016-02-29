/** @brief gripper library
 *
 * @file gripper.h
 *
 *
 * @author cpbove@wpi.edu
 * @date 27-Feb-2016
 * @version 1.0
 */

#ifndef INCLUDE_GRIPPER_H_
#define INCLUDE_GRIPPER_H_

#define GRIPPER_PIN 1
#define CONVEYOR_PIN 0

#define GRIPPER_CLOSE 170
#define GRIPPER_OPEN 10

#define CONVEYOR_FORWARD 20
#define CONVEYOR_STOP 90

/**
 * @brief Closes the gripper
 */
void closeGripper();
/**
 * @brief Opens the gripper
 */
void openGripper();
/**
 * @brief starts the conveyor belt servo
 */
void startConveyor();
/**
 * @brief stops the conveyor belt servo
 */
void stopConveyor();

#endif /* INCLUDE_GRIPPER_H_ */
