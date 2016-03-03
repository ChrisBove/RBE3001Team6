/** @brief Finite State Machine library
 *
 * @file FSM.c
 *
 *@details computes motions for the arm and monitors the conveyor belt to
 * complete the final project. It detects when a block is placed on the belt,
 * determines its X position and velocity, calculates the time the arm needs to
 * grab the block,grabs the block, determines whether the block is light or
 * heavy, then places the block accordingly and gets ready for another block.
 *
 * @author cpbove@wpi.edu
 * @date 27-Feb-2016
 * @version 1.0
 */

#include "include/FSM.h"
#include "include/arm.h"
#include "RBELib/RBELib.h"
#include "include/gripper.h"
#include "math.h"

/**
 * @brief runs FSM for the final project
 */
void finiteStateMachine(){
	static float blockStartTime = 0; // time block was first detected
	static int blockX = 0; // storing block x coordinate
	static float grabTime = 0; //time that block needs to be grabbed
	//for case CalcBlockX
	static int IRSampleMin = 999; // storing minumum distance detected by IR
	static int IRSamplesIncreasing = 0; //for counting times the distances increase
	int reading; // temporary holder for IR reading

	static char state = Initialize; // storing the state of the FSM
	switch(state){
	case Initialize:
		//reset servo positions
		startConveyor();
		openGripper();
		//reset current averages
		getAverageCurrent(resetCurrent,0);
		//place arm in high waiting position
		setPosition(Center_X,Starting_Height);
		state = WaitForBlock;
		break;

	case WaitForBlock:
		openGripper();
		//check if block is sensed on first sensor, if so, move arm to waiting
		if(IRDist(IR_FRONT_PIN) <= Distance_Threshold){
			blockStartTime = getTimeSeconds(); //save the current time
			setPosition(Center_X,Waiting_Height);
			state = CalcBlockX;//move to next state
		}
		break;

	case CalcBlockX:
		//take the lowest reading of X values with some filtering
		reading = calibratedIRVal(IRDist(IR_FRONT_PIN)); //calibrated distance
		//not done sampling until values increase consecutively(reached min)
		if(IRSamplesIncreasing < 40){
			// if reading is new min, still decreasing in values
			// note - make sure we don't get values outside of conveyor range
			if((reading <= IRSampleMin) && (reading >= 85)){
				IRSampleMin = reading; // set reading as new min
				IRSamplesIncreasing = 0; // we are not increasing
			}
			//otherwise, we have started ascending from minumum
			else
				IRSamplesIncreasing++;
		}
		else{ //done sampling
			blockX = IRSampleMin + X_IR_Offset + Fudged_X; //store block x position

			setPosition(blockX,Waiting_Height); // adjust arm towards block
			state = CalcBlockSpeed;
			IRSamplesIncreasing = 0; //reset variables
			IRSampleMin = 999;
		}
		break;
	case CalcBlockSpeed:
		// wait until 2nd sensor is toggled, calculate velocity and grab time
		if(IRDist(IR_BACK_PIN)<= Distance_Threshold){
			float deltaT = getTimeSeconds() - blockStartTime;
			float velocity = (float) Distance_Between_IR/deltaT; //mm/sec
			grabTime = Distance_IR_To_Arm/velocity + getTimeSeconds(); // time when block goes in front of arm
			state = ExecuteGrabMotion;
		}
		break;
	case ExecuteGrabMotion:
		// if we are away from grabTime by the time it takes to move, begin!
		if ((getTimeSeconds() + Time_To_Move) >= grabTime) {
			setPosition(blockX,Grab_Height); // set arm position
			state = GrabBlock;
		}
		break;
	case GrabBlock:
		// if we are away from the grab time by gripper grab time, start close!
		if ((getTimeSeconds() + Time_To_Grab) >= grabTime) {
			closeGripper();
			state = WaitForGripper;
		}
		break;
	case WaitForGripper:
		// wait until the gripper is done closing
		if(getTimeSeconds() >= grabTime + Time_To_Close) {
			state = MoveBlockUp;
		}
		break;
	case MoveBlockUp:
		// move the block upward away from conveyor
		setPosition(Center_X + 50,Waiting_Height+150);
		state = CheckWeight;
		break;
	case CheckWeight:
		//during the movment, average the currents seen on joint 2
		getAverageCurrent(addCurrent,getCurrent(2));
		//sample until reaching position
		if(doneMoving()){
			//if a heavy block, drop close, else drop far
			if(fabs(getAverageCurrent(retrieveAverageCurrent,2)) >= Heavy_Current_Threshold){
				state = GenerateTrajectoryDropClose;
			}
			else {
				state = GenerateTrajectoryDropFar;
			}
		}
		break;
	case GenerateTrajectoryDropClose:
		// move the arm to a close drop position
		setPosition(Drop_Close_X,Drop_Close_Y);
		state = ExecuteDropMotion;
		break;
	case GenerateTrajectoryDropFar:
		// move arm to a far drop position
		setPosition(Drop_Far_X,Drop_Far_Y);
		state = ExecuteDropMotion;
		break;
	case ExecuteDropMotion:
		//wait until the drop motion completes
		if(doneMoving()){
			state = DropBlock;
		}
		break;
	case DropBlock:
		// open the gripper to drop the block
		openGripper();
		state = Initialize;
		break;
	default:
		printf("Unknown FSM State!!\n\r");
		break;
	}
}
