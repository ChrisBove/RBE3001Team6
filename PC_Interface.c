/** @brief library for reading joint values from serial for extra credit
 *
 * @file PC_Interface.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 3-March-2016
 * @version 1.0
 */

#include "include/PC_Interface.h"
#include "string.h"
#include "stdlib.h"
#include "include/arm.h"
#include "include/lab1.h"


/**
 * @brief returns the converted float from serial line
 * @details expects format:
 * 		a34.58,
 * 		b23.284,
 * @return joint angle read from serial line
 */
float getJointCommand(int joint){
	int i = 0; // index counter for stepping through string
	char input[10]; //storage of parsed input from serial.
	//based on selected joint, wait for its designated char over serial
	if(joint == 1)
		waitForChar('a');
	else
		waitForChar('b');

	// read number values until receiving a ','
	input[i] = getCharDebug();
	while (input[i] != ',') {
		i++;
		input[i] = getCharDebug();
	}
	input[i] = 0; // overwrite comma to 0
	// convert string to number
	return atof(input);
}
/**
 * @brief controls the arm joints with values read from serial
 * @note this should be called at least at 100hz to allow PID to function as it
 *  will block other functions until the read completes.
 */
void controlArmWithSerial(){
	setJointAngles(getJointCommand(1),getJointCommand(2));
}
