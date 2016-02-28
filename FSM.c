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
	static float blockStartTime = 0;
	static int blockX = 0;
	static float grabTime = 0;

	static char state = Initialize;
	switch(state){
	case Initialize:
		startConveyor();
		openGripper();
		//TODO setXY(CenterX,Waiting_Height+100);
		state = WaitForBlock;
		break;

	case WaitForBlock:
		//check if block is sensed (below 110mm)
		if(IRDist(0) <= Distance_Threshold){
			blockStartTime = getTimeSeconds(); //save the current time
			//TODO setXY(CenterX,Waiting_Height);
			setJointAngles(45,80); //start moving the arm to a nominal position
			state = CalcBlockX;//move to next state
		}
		break;

	case CalcBlockX:
		static int count = 0;
		static int distanceSum = 0;

		// sample the 1st sensor a few times
		if(count < 3){
			distanceSum += IRDist(0); // add values
			count++;
		}
		else{
			blockX = distanceSum/count;
			//todo setXY(blockX+CenterX,Waiting_Height);
			state = CalcBlockSpeed;
			distanceSum = 0;
			count = 0;
		}
		break;
	case CalcBlockSpeed:
		// wait until 2nd sensor is toggled
		if(IRDist(1)<= Distance_Threshold){
			float deltaT = getTimeSeconds() - blockStartTime;
			float velocity = (float) Distance_Between_IR/deltaT; //mm/sec
			grabTime = Distance_IR_To_Arm/velocity; // time until block goes in front of arm
			state = GenerateTrajectoryGrab;
		}
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