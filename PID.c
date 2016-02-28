/**
 * @file PID.c
 *
 * @brief The source file for PID constants and calculations.
 * @details Sets the PID constants and calculate the PID value.
 * @author Chris Bove
 * @date 2-3-16
 */

#include "RBELib/RBELib.h"
#include "include/definitions.h"
#include "math.h"

/**
* @brief Declaration for use in other files.
*/
pidConst pidConsts;

/**
 * @brief Sets the Kp, Ki, and Kd values for 1 link.
 * @param link The link you want to set the values for (2 or 3).
 * @param Kp Proportional value.
 * @param Ki Integral value.
 * @param Kd Derivative value.
 *
 */
void setConst(char link, float Kp, float Ki, float Kd){
	if(link == 3) {
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
	}
	else{
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
	}

}

/**
 * @brief Calculate the PID value.
 * @param  link Which link to calculate the error for (Use 'U' and 'L').
 * @param setPoint The desired position of the link.
 * @param actPos The current position of the link.
 *
 */
signed int calcPID(char link, int setPoint, int actPos){
	// ==== define static variables ====
	// todo change some of these to constants.
	// todo make these arrays or something nicer and more modular

	// storing error summations
	static int errorSumLink2 = 0;
	static int errorSumLink3 = 0;
	// storing the last errors
	static int lastErrorLink2 = 0;
	static int lastErrorLink3 = 0;
	// holding a count of how many loops were completed to clear errorSums
	static unsigned int loopCountLink2 = 0;
	static unsigned int loopCountLink3 = 0;
	// store the last setpoint to see when updates occur and reset vars
	static int lastSetPointLink2 = 0;
	static int lastSetPointLink3 = 0;
	// constants for static torque for links
	static const int link2StaticTorque = 150;
	static const int link3StaticTorque = 200;
	// constants for gravity and link masses (arbitrary values)
	static const int gravity = 10;
	static const int link2Mass = 15;
	static const int link3Mass = 3;
	// ==== END declarations ====

	int currentError = setPoint - actPos; // calculate the current error

	signed int output;
	// if we are not within +-2, compute new output
	// todo make this 2 value a constant defined somewhere
	if(!betweenTwoVals(currentError,-2,2)){
		// if on link 2, compute new output for that link
		// todo this is gross. Make this use an array or something so it isn't repeated
		if (link == 2) {
			// if the setpoint changed, reset variables and continue
			if (lastSetPointLink2 != setPoint) {
				errorSumLink2 = 0;
				loopCountLink2 = 0;
				lastErrorLink2 = 0;
				lastSetPointLink2 = setPoint;
			}

			// main output calculation: Kp, Ki, Kd, g, torqueConst
			output = pidConsts.Kp_L * (currentError)
					+ pidConsts.Ki_L * (errorSumLink2)
					+ pidConsts.Kd_L * (currentError - lastErrorLink2)
					// feed forward gravity compensation
					+ (cos(actPos * RADS_PER_DEGREE) * gravity * link2Mass);
			// add static torque based on direction we want to go in
			if (currentError < 0) {
				output -= link2StaticTorque;
			} else {
				output += link2StaticTorque;
			}

			lastErrorLink2 = currentError; // update last error
			// if number of runs is over 5000, reset the sum.
			// todo make this not a magic number
			if (loopCountLink2 > 5000) {
				errorSumLink2 = 0;
			} else {
				// otherwise, add to the summation
				errorSumLink2 += currentError;
			}

			loopCountLink2++; // increment the loop counter


		} else { // link 3
			// if the setpoint changed, reset variables and continue
			if (lastSetPointLink3 != setPoint) {
				errorSumLink3 = 0;
				loopCountLink3 = 0;
				lastErrorLink3 = 0;
				lastSetPointLink3 = setPoint;
			}

			// main output calculation: Kp, Ki, Kd, g, torqueConst
			output = pidConsts.Kp_H * (currentError)
					+ pidConsts.Ki_H * (errorSumLink3)
					+ pidConsts.Kd_H * (currentError - lastErrorLink3)
					// feed forward gravity compensation based on angle with horizontal
					+ (cos((actPos-(90-lastJoint1Angle))*RADS_PER_DEGREE)*gravity*link3Mass);

			// add static torque based on direction we want to go in
			if(currentError < 0) {
				output -= link3StaticTorque;
			}
			else {
				output += link3StaticTorque;
			}

			lastErrorLink3 = currentError; // update last error

			// if number of runs is over 5000, reset the sum.
			// todo make this not a magic number
			if (loopCountLink3 > 5000) {
				errorSumLink3 = 0;
			} else {
				errorSumLink3 += currentError;
			}

			loopCountLink3++; // increment the loop counter
		}
	} else {
		// we're basically there, so just stop doing whatever you are.
		//zero variables
		if(link == 2){
			errorSumLink2 = 0;
			loopCountLink2 = 0;
			lastErrorLink2 = 0;
			lastSetPointLink2 = 0;
		}
		else {
			errorSumLink3 = 0;
			loopCountLink3 = 0;
			lastErrorLink3 = 0;
			lastSetPointLink3 = 0;
		}
		output = 0; // set output to 0

	}

	return output;
}
