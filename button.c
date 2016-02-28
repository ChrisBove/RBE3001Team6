/** @brief button setup function
 *
 * @file button.c
 *
 * To read buttons, check PINxbits._Py
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/button.h"

/**
 * @var *PINybits
 * pointer to the PINbits the buttons are connected to
 */
__8bitreg_t *PINybits;

/**
 * @var b4
 * storing last state of button 4
 *
 * @var b5
 * storing last state of button 5
 *
 * @var b6
 * storing last state of button 6
 *
 * @var b7
 * storing last state of button 7
 *
 * @todo make a nice array or something
 * @todo make for rest of buttons
 */
unsigned char b4;
unsigned char b5;
unsigned char b6;
unsigned char b7;

/**
 * @brief sets registers to configure the buttons as inputs
 *
 * @param *DDxn Pointer to the digital port (DDxn) that buttons are connected to
 * @param *PORTxn Pointer to the PORTxn register for the buttons
 * @param *PINxbits Pointer to the PINx register for the buttons
 * @param enablePullUp Set true to enable pull up resistors on the buttons
 */
void setupButtons(__8bitreg_t *DDxn, __8bitreg_t *PORTxn, __8bitreg_t *PINxbits, BOOL enablePullup){
	PINybits = PINxbits; // update global
	//if pullups are enabled, set them to be pulled up
	if(enablePullup){
		PORTxn->_P0 = HIGH;
		PORTxn->_P1 = HIGH;
		PORTxn->_P2 = HIGH;
		PORTxn->_P3 = HIGH;
		//PORTxn->_P4 = HIGH;
		//PORTxn->_P5 = HIGH;
		PORTxn->_P6 = HIGH;
		PORTxn->_P7 = HIGH;
	}
	// set all buttons as inputs
	DDxn->_P0 = INPUT;
	DDxn->_P1 = INPUT;
	DDxn->_P2 = INPUT;
	DDxn->_P3 = INPUT;
	//DDxn->_P4 = INPUT;
	//DDxn->_P5 = INPUT;
	DDxn->_P6 = INPUT;
	DDxn->_P7 = INPUT;
	// service the buttons once
	serviceButtons();
}

/**
 * @brief runs functions critical to button operation. Call as often as possible.
 */
void serviceButtons(){
	// updates globals with latest pin states
	b4 = PINybits->_P0;
	b5 = PINybits->_P1;
	b6 = PINybits->_P2;
	b7 = PINybits->_P3;
}

/**
 * @brief determines what button was pressed last
 *
 * @return number of last button pressed
 */
unsigned char lastButtonPressed(){
	// needed static variables
	// todo make arrays
	static unsigned char lastButtonPressed = 4; // last button pressed
	static unsigned char b4LastState = 1; // storage of last button state
	static unsigned char b5LastState = 2;
	static unsigned char b6LastState = 1;
	static unsigned char b7LastState = 1;

	//switch based on which button was last pressed
	if (b4LastState != b4) {
		lastButtonPressed = 4;
		b4LastState = b4;
	} else if (b5LastState != b5) {
		lastButtonPressed = 5;
		b5LastState = b5;
	} else if (b6LastState != b6) {
		lastButtonPressed = 6;
		b6LastState = b6;
	} else if (b7LastState != b7) {
		lastButtonPressed = 7;
		b7LastState = b7;
	}
	return lastButtonPressed;
}
