/** @brief LED setup function definitions
 *
 * @file led.h
 *
 * This has a function to setup a port register as an output,
 * specifically useful for doing the LED's on the board.
 * I'm not sure why, but I originally thought we needed functions
 * to turn them on and off too, but that takes about as much effort
 * as just writing to the port directly, so this file just has
 * setup stuff.
 *
 * To toggle: use PINxn = 1 to toggle the value of PORTxn
 * To set, just use PORTxn = 0 or 1
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#ifndef INCLUDE_LED_H_
#define INCLUDE_LED_H_

#include"RBELib/RBELib.h"

/**
 * @brief configures registers as outputs for the LEDs
 *
 * @param *DDxn Pointer to the digital outputs (DDxn) that buttons will be connected to
 * @param *PORTxn Pointer to the PORTxn that actually writes the outputs
 * @param enablePullUp Set true to enable pull ups on the buttons
 */
void setupLEDs(__8bitreg_t *DDxn, __8bitreg_t *PORTxn);

#endif /* INCLUDE_LED_H_ */
