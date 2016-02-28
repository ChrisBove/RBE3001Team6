/** @brief Finite State Machine library
 *
 * @file FSM.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 27-Feb-2016
 * @version 1.0
 */

#include "include/FSM.h"
#include "include/arm.h"
#include "RBELib/RBELib.h"
#include "include/gripper.h"

/**
 * @brief runs FSM for the final project
 */
void finiteStateMachine(){
	static char state = Initialize;
	switch(state){
	case Initialize:
		startConveyor();
		openGripper();
		state = WaitForBlock;
		break;

	case WaitForBlock:

		break;

	case CalcBlockStatus:

		break;
	case GenerateTrajectoryGrab:

		break;
	case ExecuteGrabMotion:

		break;
	case GrabBlock:

		break;
	case CheckWeight:

		break;
	case GenerateTrajectoryDropClose:

		break;
	case GenerateTrajectoryDropFar:

		break;
	case ExecuteDropMotion:

		break;
	case DropBlock:

		break;
	default:
		printf("Unknown FSM State!!\n\r");
		break;
	}
}
