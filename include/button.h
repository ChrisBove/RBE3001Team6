/** @brief button setup function definitions
 *
 * @file button.h
 *
 * This has a function to setup a port register as inputs,
 * specifically useful for doing the switches on the board.
 * I'm not sure why, but I originally thought we needed functions
 * to read them too, but that takes about as much effort
 * as just reading the port directly, so this file just has
 * setup stuff and the old start of button reading.
 *
 * Maybe eventually this could be expanded arduino style with
 * numbered pins.
 *
 * To read buttons, check PINxbits._Py
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include "RBELib/RBELib.h"

/**
 * @brief sets registers to configure the buttons as inputs
 *
 * @param *DDxn Pointer to the digital port (DDxn) that buttons are connected to
 * @param *PORTxn Pointer to the PORTxn register for the buttons
 * @param enablePullUp Set true to enable pull up resistors on the buttons
 */
void setupButtons(__8bitreg_t *DDxn, __8bitreg_t *PORTxn, __8bitreg_t *PINxbits, BOOL enablePullup);
/**
 * @brief runs functions critical to button operation. Call as often as possible.
 */
void serviceButtons();

/**
 * @brief determines what button was pressed last
 *
 * @return number of last button pressed
 */
unsigned char lastButtonPressed();

#endif /* INCLUDE_BUTTON_H_ */
