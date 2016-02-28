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
	CalcBlockStatus,
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
 * @brief runs FSM for the final project
 */
void finiteStateMachine();

#endif /* INCLUDE_FSM_H_ */
