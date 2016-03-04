/** @brief Finite State Machine library for solving 3001 Final Project
 *
 * @file FSM.h
 *
 * @details computes motions for the arm and monitors the conveyor belt to
 * complete the final project. It detects when a block is placed on the belt,
 * determines its X position and velocity, calculates the time the arm needs to
 * grab the block,grabs the block, determines whether the block is light or
 * heavy, then places the block accordingly and gets ready for another block.
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

/**
 * @def IR_FRONT_PIN
 * the analog pin for the first/front IR sensor on the conveyor
 * @def IR_BACK_PIN
 * the analog pin for the last/backIR sensor on the conveyor
 */
#define IR_FRONT_PIN 4
#define IR_BACK_PIN 5

/**
 * @def X_Spacer
 * the width of the spacer added between the arm and the conveyor belt to stop
 * the arm from reaching joint limits when picking up blocks from the close edge
 * @def Waiting_Height
 * height in mm arm will wait above conveyor belt once block is detected
 * @def Starting_Height
 * height the arm will initialize to
 * @def Center_X
 * x coordinate arm will wait at once block is detected. Center of conveyor
 * @def Grab_Height
 * height that the arm will drop to for picking up the block
 *
 */
#define X_Spacer 31.43
#define Waiting_Height 151
#define Starting_Height Waiting_Height+100
#define Center_X 196
#define Grab_Height 120
/**
 * @def Drop_Far_X
 * the x coordinate for dropping the block at a far distance
 * @def Drop_Far_Y
 * the Y coordinate for dropping the block at a far distance
 * @def Drop_Close_X
 * the x coordinate for dropping the block at a close distance
 * @def Drop_Close_Y
 * the y coordinate for dropping the block at a close distance
 */
#define Drop_Far_X 300
#define Drop_Far_Y Waiting_Height+50
#define Drop_Close_X 120
#define Drop_Close_Y Waiting_Height+100
/**
 * @def Fudged_X
 * a value compensating for x error seen in the detection system
 */
#define Fudged_X 13
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
#define X_IR_Offset 76.81 + X_Spacer

/**
 * @def Time_To_Move
 * The time before the block grab time to start moving down
 * @def Time_To_Grab
 * The time before the block grab time to request a gripper close
 * @def Time_To_Close
 * The time the gripper needs to firmly close around the block
 */
#define Time_To_Move -0.3
#define Time_To_Grab -0.55
#define Time_To_Close 0.9

/**
 * @def Heavy_Current_Threshold
 * currents higher than this mean we lifted a heavy block
 */
#define Heavy_Current_Threshold 645

/**
 * @brief runs FSM for the final project
 */
void finiteStateMachine();

#endif /* INCLUDE_FSM_H_ */
