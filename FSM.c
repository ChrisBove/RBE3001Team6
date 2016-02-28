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
	//for case CalcBlockX
	static int IRSampleCount = 0;
	static int IRDistanceSum = 0;

	static char state = Initialize;
	switch(state){
	case Initialize:
		startConveyor();
		openGripper();
		//TODO setPosition(CenterX,Waiting_Height+100);
		state = WaitForBlock;
		break;

	case WaitForBlock:
		//check if block is sensed (below 110mm)
		if(IRDist(0) <= Distance_Threshold){
			blockStartTime = getTimeSeconds(); //save the current time
			//TODO setPosition(CenterX,Waiting_Height);
			setJointAngles(45,80); //start moving the arm to a nominal position
			state = CalcBlockX;//move to next state
		}
		break;

	case CalcBlockX:
		// sample the 1st sensor a few times
		if(IRSampleCount < 3){
			IRDistanceSum += IRDist(0); // add values
			IRSampleCount++;
		}
		else{
			blockX = IRDistanceSum/IRSampleCount;
			//todo setPosition(blockX+CenterX,Waiting_Height);
			state = CalcBlockSpeed;
			IRDistanceSum = 0;
			IRSampleCount = 0;
		}
		break;
	case CalcBlockSpeed:
		// wait until 2nd sensor is toggled
		if(IRDist(1)<= Distance_Threshold){
			float deltaT = getTimeSeconds() - blockStartTime;
			float velocity = (float) Distance_Between_IR/deltaT; //mm/sec
			grabTime = Distance_IR_To_Arm/velocity + getTimeSeconds(); // time when block goes in front of arm
			state = ExecuteGrabMotion;
		}
		break;
	case GenerateTrajectoryGrab:

		break;
	case ExecuteGrabMotion:
		// if we are away from grabTime by the time it takes to move, begin!
		if ((getTimeSeconds() + Time_To_Move) >= grabTime) {
			//todo setPosition(blockX+CenterX,Grab_Height);
			state = GrabBlock;
		}
		break;
	case GrabBlock:
		if ((getTimeSeconds() + Time_To_Grab) >= grabTime) {
			closeGripper();
			state = WaitForGripper;
		}
		break;
	case WaitForGripper:
		if(getTimeSeconds() >= grabTime + Time_To_Close) {
			state = CheckWeight;
		}
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
