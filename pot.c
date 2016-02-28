/**
 * @file pot.c
 *
 * @brief The potentiometer functions.
 * @details  Use these functions to read the values from the pots.
 * @author Chris Bove
 * @date 2-3-16
 */

#include "RBELib/RBELib.h"

/**
 * @brief Find the angle of the given potentiometer.
 * @param  pot The pot to check.
 * @return angle Angle of the potentiometer.
 *
 */
int potAngle(int pot){
	return (300.0 * getADC(pot))/1023.0;
}

/**
 * @brief Find the voltage value of the given potentiometer.
 * @param  pot The pot to get the value of.
 * @return volts Voltage of  potentiometer.
 *
 */
int potVolts(int pot){
	return (5000.0 * getADC(pot))/1023.0;
}
