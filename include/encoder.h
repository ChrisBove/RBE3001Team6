/** @brief encoder library
 *
 * @file encoder.h
 *
 *
 * @author LSI Computer Systems, modified by cpbove@wpi.edu
 * @date 16-Feb-2016
 * @version 1.0
 */

#ifndef INCLUDE_ENCODER_H_
#define INCLUDE_ENCODER_H_

#include "RBELib/RBELib.h"

/**
 *
 * @def CLR_CNTR
 * op-code for clearing the encoder count. used with singleByteWrite
 */
#define CLR_CNTR	0x20

/**
 * @brief write a single byte to encoder
 * @param op_code one of the defined operation codes
 * @param data data to write to the encoder
 * @param joint 0 or 1 for the encoder joint to read
 */
void singleByteWrite(unsigned char op_code, unsigned char data, int joint);
/**
 * @brief sets the chip select low for the passed joint
 * @param joint 0 or 1 to select
 */
void slaveSelect(int joint);
/**
 * @brief sets the chip select high for the passed joint
 * @param joint 0 or 1 to deselect
 */
void slaveDeselect(int joint);
/**
 * @brief holds processor until transmission has finished
 */
void waitForTransmissionEnd();


#endif /* INCLUDE_ENCODER_H_ */
