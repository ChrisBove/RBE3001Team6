/** @brief Finite State Machine library
 *
 * @file FSM.h
 *
 *
 * @author cpbove@wpi.edu
 * @date 27-Feb-2016
 * @version 1.0
 */

#ifndef INCLUDE_FSM_H_
#define INCLUDE_FSM_H_

/**
 * @enum FSMStates
 * For tracking states of the FSM
 */
enum FSMStates {
	Initialize,
	WaitForBlock,
	CalcBlockX,
	CalcBlockSpeed,
	GenerateTrajectoryGrab,
	ExecuteGrabMotion,
	GrabBlock,
	CheckWeight,
	GenerateTrajectoryDropFar,
	GenerateTrajectoryDropClose,
	ExecuteDropMotion,
	DropBlock
};

/**
 * @def Waiting_Height
 * height in mm arm will wait above conveyor belt once block is detected
 * @def Center_X
 * x coordinate arm will wait at once block is detected
 */
#define Waiting_Height 110
#define Center_X 125

/**
 * @def Distance_Threshold
 * distance in mm to trip the IR sensor
 * @def Distance_Between_IR
 * distance in mm between IR sensors
 * @def Distance_IR_To_Arm
 * distance in mm between last IR sensor and Arm
 */
#define Distance_Threshold 110
#define Distance_Between_IR 100
#define Distance_IR_To_Arm 250

/**
 * @def Time_To_Move
 * The time before the block grab time to start moving down
 * @def Time_To_Grab
 * The time before the block grab time to request a gripper close
 */
#define Time_To_Move 1.0
#define Time_To_Grab 0.4

/**
 * @brief runs FSM for the final project
 */
void finiteStateMachine();

#endif /* INCLUDE_FSM_H_ */
