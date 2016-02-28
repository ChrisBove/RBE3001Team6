/** @brief LED setup functions
 *
 * @file led.c
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#include "include/led.h"
#include "RBELib/RBELib.h"

/**
 * @brief configures registers as outputs for the LEDs
 *
 * Sets all the ports as outputs and then puts them all low to prevent them
 * from turning on unintentionally.
 *
 * @param *DDxn Pointer to the digital outputs (DDxn) that buttons will be connected to
 * @param *PORTxn Pointer to the PORTxn that actually writes the outputs
 * @param enablePullUp Set true to enable pull ups on the buttons
 */
void setupLEDs(__8bitreg_t *DDxn, __8bitreg_t *PORTxn){
	DDxn->_P0 = OUTPUT;
	DDxn->_P1 = OUTPUT;
	DDxn->_P2 = OUTPUT;
	DDxn->_P3 = OUTPUT;
	DDxn->_P4 = OUTPUT;
	DDxn->_P5 = OUTPUT;
	DDxn->_P6 = OUTPUT;
	DDxn->_P7 = OUTPUT;
	// set all to no output, which is HIGH
	PORTxn->_P0 = HIGH;
	PORTxn->_P1 = HIGH;
	PORTxn->_P2 = HIGH;
	PORTxn->_P3 = HIGH;
	PORTxn->_P4 = HIGH;
	PORTxn->_P5 = HIGH;
	PORTxn->_P6 = HIGH;
	PORTxn->_P7 = HIGH;
	// todo add way to allow selecting between sink and source
}
