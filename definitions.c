/** @brief defines for our team
 *
 * @file definitions.c
 *
 * some basic useful functions that our team uses.
 *
 *
 * @author cpbove@wpi.edu
 * @date 3-Feb-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/definitions.h"

/**
 * @brief take absolute value of passed integer
 * @param  a The number to take the absolute value of
 *
 */
int abs(int a){
	if(a < 0)
		return -1*a;
	else
		return a;
}

/**
 * @brief check if passed value is between lower and upper bounds
 * @param  value the number to check with
 * @param  lower the lower bound to check against
 * @param  upper the upper bound to check against
 *
 * @return true if value is between upper and lower numbers
 */
BOOL betweenTwoVals(int value, int lower, int upper){
	return (value <= upper && value >= lower);
}
