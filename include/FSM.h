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
	WaitForGripper,
	MoveBlockUp,
	CheckWeight,
	GenerateTrajectoryDropFar,
	GenerateTrajectoryDropClose,
	ExecuteDropMotion,
	DropBlock
};

#define IR_FRONT_PIN 4
#define IR_BACK_PIN 5

/**
 * @def Waiting_Height
 * height in mm arm will wait above conveyor belt once block is detected
 * @def Center_X
 * x coordinate arm will wait at once block is detected
 *
 */
#define Waiting_Height 145
#define Center_X 196
#define Grab_Height 124
#define Drop_Far_X 300
#define Drop_Close_X 120

/**
 * @def Distance_Threshold
 * distance in mm to trip the IR sensor
 * @def Distance_Between_IR
 * distance in mm between IR sensors
 * @def Distance_IR_To_Arm
 * distance in mm between last IR sensor and Arm
 * @def X_IR_Offset
 * additional distance from IR sensors to Arm frame origin
 */
#define Distance_Threshold 200
#define Distance_Between_IR 64.7
#define Distance_IR_To_Arm 130
#define X_IR_Offset 76.81

/**
 * @def Time_To_Move
 * The time before the block grab time to start moving down
 * @def Time_To_Grab
 * The time before the block grab time to request a gripper close
 */
#define Time_To_Move 0.00
#define Time_To_Grab -0.5
#define Time_To_Close 0.9

/**
 * @def Heavy_Current_Threshold
 * currents higher than this mean we lifted a heavy block
 */
#define Heavy_Current_Threshold 573

/**
 * @brief runs FSM for the final project
 */
void finiteStateMachine();

#endif /* INCLUDE_FSM_H_ */
