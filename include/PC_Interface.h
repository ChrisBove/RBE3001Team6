/** @brief library for reading joint values from serial for extra credit
 *
 * @file PC_Interface.h
 *
 *
 * @author cpbove@wpi.edu
 * @date 3-March-2016
 * @version 1.0
 */

#ifndef INCLUDE_PC_INTERFACE_H_
#define INCLUDE_PC_INTERFACE_H_

/**
 * @brief returns the converted float from serial line
 * @details expects format:
 * 		a34.58,
 * 		b23.284,
 * @return joint angle read from serial line
 */
float getJointCommand(int joint);
/**
 * @brief controls the arm joints with values read from serial
 * @note this should be called at least at 100hz to allow PID to function as it
 *  will block other functions until the read completes.
 */
void controlArmWithSerial();

#endif /* INCLUDE_PC_INTERFACE_H_ */
