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

/**
 * @def GRIPPER_PIN
 * pin on the co-processor that the gripper servo is connected to
 * @def CONVEYOR_PIN
 * pin on the co-processor that the conveyor motor is connected to
 */
#define GRIPPER_PIN 2
#define CONVEYOR_PIN 0
/**
 * @def GRIPPER_CLOSE
 * servo value for gripper to close it
 * @def GRIPPER_OPEN
 * servo value for gripper to open it
 */
#define GRIPPER_CLOSE 170
#define GRIPPER_OPEN 0
/**
 * @def CONVEYOR_FORWARD
 * servo value for making conveyor go forward
 * @def CONVEYOR_STOP
 * servo value for making conveyor stop
 */
#define CONVEYOR_FORWARD 0
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
