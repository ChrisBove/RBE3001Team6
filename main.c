/** @brief Main loop running on board. Runs Lab1 functions.
 *
 * @file main.c
 *
 * This code runs a collection of Lab 1 functions to complete the lab.
 * Some functions need to be uncommented or commented depending on which
 * section they complete. This should be more streamlined in the future.
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

// includes
#include "RBELib/RBELib.h"
#include "include/definitions.h" // our definitions and macros
#include "include/button.h"
#include "include/lab1.h" // lab1 functions
#include "include/led.h"
#include "include/arm.h"
#include "include/FSM.h"
#include "include/gripper.h"
#include "string.h"
#include "stdlib.h"

/**
 * @brief main loop for AVR chip
 *
 */
int main(void) {
	// ==== do initializations!! ===
	initRBELib(); // allows printf + more to work
	debugUSARTInit(OUR_BAUD_RATE); // intialize USART communications
	printf("Starting...\n\r"); // so we know if we freeze in setup

	// setup buttons on port C.
	//setupButtons(&DDRCbits, &PORTCbits, &PINCbits, FALSE);

	initSPI(); // initialize SPI communications
	initArm(); // initialize the arm
	stopConveyor();
	openGripper();
	//encInit(1);
	//encInit(2);
	//getAccel(0); // run once to initialize Vref


	// ==== end initializations ====

	printf("I am alive... Press h to home.\n\r"); // we done setting up
//	waitForChar('h');
//	printf("homing...\n\r");
	//homePos(); // home to initial pose
	//resetEncCount(1);
	//resetEncCount(2);
//	printf("Done homing. Press g to continue.\n\r");
//	waitForChar('g');

	// ===== main loop ====
	while (1) {
		//serviceButtons(); // service the buttons for polling
//		printf("IR: %i %i\n\r",IRDist(4),IRDist(5));
//		static int count = 0;
//		if (count > 200){
//			printf("%.1f,%.1f\n\r",getJointAngle(1),getJointAngle(2));
//			count = 0;
//		}
//		else
//			count++;
//		finiteStateMachine();
		int i = 0;
		char input[10];
		// wait until getting an a
		waitForChar('a');
//		waitForChar(' ');

		// read until receiving a ,
		input[i] = getCharDebug();
		while (input[i] != ',') {
			i++;
			input[i] = getCharDebug();
		}
		input[i] = 0; // overright comma to 0
		// convert string to number
		float joint1 = atof(input);

		int i2 = 0;
		char input2[10];
		//wait until getting a b
		waitForChar('b');
//		waitForChar(' ');
		input2[i2] = getCharDebug();
		while (input2[i2] != ',') {
			i2++;
			input2[i2] = getCharDebug();
		}
		input2[i2] = 0; // overright comma to 0
		// convert string to number
		float joint2 = atof(input2);
		// set number to arm angle
		setJointAngles(joint1,joint2);
		serviceArm();
//		while(!doneMoving()){
//			serviceArm(); // service the arm
//		}
//		stopMotors();

//		_delay_ms(100); // prevent serial spam

	}
	return 0;
}
