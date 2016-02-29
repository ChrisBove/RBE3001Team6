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
	static int IRSampleMin = 999;
	static int IRSamplesIncreasing = 0;
	int reading; // temporary holder for IR reading

	static char state = Initialize;
	switch(state){
	case Initialize:
		printf("init\n\r");
		startConveyor();
		openGripper();
		setPosition(Center_X,Waiting_Height+100);
		state = WaitForBlock;
		break;

	case WaitForBlock:
		printf("wait\n\r");
		openGripper();
		//check if block is sensed (below 110mm)
		if(IRDist(IR_FRONT_PIN) <= Distance_Threshold){
			blockStartTime = getTimeSeconds(); //save the current time
			setPosition(Center_X,Waiting_Height);
			//setJointAngles(45,80); //start moving the arm to a nominal position
			state = CalcBlockX;//move to next state
		}
		break;

	case CalcBlockX:
		printf("calcBlock\n\r");
		//take the lowest reading with some filtering
		reading = IRDist(IR_FRONT_PIN);
		//not done sampling until values increase consecutively(reached min)
		if(IRSamplesIncreasing < 50){
			// if reading is new min, still decreasing in values
			if(reading <= IRSampleMin){
				IRSampleMin = reading;
				IRSamplesIncreasing = 0;
			}
			//otherwise, we have started ascending from minumum
			else
				IRSamplesIncreasing++;
		}
		else{ //done sampling
			blockX = IRSampleMin + X_IR_Offset; //store block x position

			setPosition(blockX,Waiting_Height);
			state = CalcBlockSpeed;
			IRSamplesIncreasing = 0; //reset variables
			IRSampleMin = 999;
		}
		break;
	case CalcBlockSpeed:
		printf("calcSpeed\n\r");
		// wait until 2nd sensor is toggled
		if(IRDist(IR_BACK_PIN)<= Distance_Threshold){
			float deltaT = getTimeSeconds() - blockStartTime;
			float velocity = (float) Distance_Between_IR/deltaT; //mm/sec
			grabTime = Distance_IR_To_Arm/velocity + getTimeSeconds(); // time when block goes in front of arm
			state = ExecuteGrabMotion;
		}
		break;
	case GenerateTrajectoryGrab:

		break;
	case ExecuteGrabMotion:
		printf("grabmove\n\r");
		// if we are away from grabTime by the time it takes to move, begin!
		if ((getTimeSeconds() + Time_To_Move) >= grabTime) {
			setPosition(blockX,Grab_Height);
			state = GrabBlock;
		}
		break;
	case GrabBlock:
		printf("grip\n\r");
		if ((getTimeSeconds() + Time_To_Grab) >= grabTime) {
			closeGripper();
			state = WaitForGripper;
		}
		break;
	case WaitForGripper:
		printf("wait\n\r");
		if(getTimeSeconds() >= grabTime + Time_To_Close) {
			state = CheckWeight;
		}
		break;
	case CheckWeight:
		printf("check\n\r");
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
