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

	printf("I am alive... You're about to see some serious !@#$!\n\r");

	// ===== main loop ====
	while (1) {
		finiteStateMachine();
		serviceArm();
	}
	return 0;
}
