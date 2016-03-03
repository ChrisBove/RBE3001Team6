/** @brief defines for our team
 *
 * @file definitions.h
 *
 * Includes defines that our team uses.
 *
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

/**
 * @var PID
 * for grabbing the last PID value calculated for logging
 * @var lastJoint1Angle
 * for getting bottom joint for calculating grav comp for top link
 */
int PID;
int lastJoint1Angle;

/**
 * @brief take absolute value of passed integer
 * @param  a The number to take the absolute value of
 *
 * @return absolute value of number
 */
int abs(int a);

/**
 * @brief check if passed value is between lower and upper bounds
 * @param  value the number to check with
 * @param  lower the lower bound to check against
 * @param  upper the upper bound to check against
 *
 * @return true if value is between upper and lower numbers
 */
BOOL betweenTwoVals(int value, int lower, int upper);

#ifndef INCLUDE_DEFINITIONS_H_
#define INCLUDE_DEFINITIONS_H_

/*
 * @def OUR_BAUD_RATE
 * The baudrate for USART to the PC
 */
#define OUR_BAUD_RATE 115200

/**
 * @def BIT(n)
 * select bit n assuming n is an int and 0 <= n <= 7
 *
 * @def F_CLOCK
 * Frequency of our clock
 */
#define BIT(n) (1 << (n))
#define F_CLOCK 18432000UL
/**
 * @def JOINT_2_DAC_0
 * DAC channel that joint 1 DAC 0 is connected to
 *
 * @def JOINT_2_DAC_1
 * DAC channel that joint 1 DAC 1 is connected to
 *
 * @def JOINT_1_DAC_0
 * DAC channel that joint 0 DAC 0 is connected to
 *
 * @def JOINT_1_DAC_0
 * DAC channel that joint 0 DAC 1 is connected to
 */
#define JOINT_2_DAC_0 2
#define JOINT_2_DAC_1 3
#define JOINT_1_DAC_0 0
#define JOINT_1_DAC_1 1

/**
 * @def RADS_PER_DEGREE
 * number of radians per degree for conversions
 * @def DEGREES_TO_RADIANS
 * number of degrees in 1 radian
 */
#define RADS_PER_DEGREE	0.01745
#define DEGREES_TO_RADIANS 57.2958

/**
 * @def X_AXIS
 * DAC channel that joint 1 DAC 0 is connected to
 *
 * @def Y_AXIS
 * DAC channel that joint 1 DAC 1 is connected to
 *
 * @def Z_AXIS
 * DAC channel that joint 0 DAC 0 is connected to

 */
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#endif /* INCLUDE_DEFINITIONS_H_ */
