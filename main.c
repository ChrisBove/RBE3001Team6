/** @brief Main loop running on board. Runs Lab1 functions.
 *
 * @file main.c
 *
 * This code runs initialization routines and then loops the finite state
 * machine and arm service routines forever to complete the final project for
 * RBE 3001.
 *
 * @author cpbove@wpi.edu
 * @date 3-Mar-2016
 * @version 1.5
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
#include "include/PC_Interface.h"

/**
 * @brief main loop for AVR chip
 *
 */
int main(void) {
	// ==== do initializations!! ===
	initRBELib(); // allows printf + more to work
	debugUSARTInit(OUR_BAUD_RATE); // intialize USART communications
	printf("Starting...\n\r"); // so we know if we freeze in setup

	initSPI(); // initialize SPI communications
	initArm(); // initialize the arm'

	stopConveyor(); // initialize servo positions
	openGripper();

	// ==== end initializations ====

	printf("I am alive... Looking for blocks to pickup.\n\r");

	// ===== main loop ====
	while (1) {
		finiteStateMachine(); // run FSM to determine what arm needs to do
		serviceArm(); // allow arm to react to changes and service PID if needed
	}
	return 0;
}
