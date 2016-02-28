/** @brief encoder library
 *
 * @file encoder.c
 *
 *
 * @author LSI Computer Systems, modified by cpbove@wpi.edu
 * @date 16-Feb-2016
 * @version 1.0
 */

#include "include/encoder.h"
#include "RBELib/RBELib.h"
#include "include/definitions.h"

/**
 * @brief write a single byte to encoder
 * @param op_code one of the defined operation codes
 * @param data data to write to the encoder
 * @param joint 1 or 2 for the encoder joint to read
 */
void singleByteWrite(unsigned char op_code, unsigned char data, int joint){
	slaveDeselect(joint); // deselect
	slaveSelect(joint); // select the chip

	spiTransceive(op_code);
	spiTransceive(data);

	slaveDeselect(joint); // deselect chip

}

/**
 * @brief sets the chip select low for the passed joint
 * @param joint 1 or 2 to select
 */
void slaveSelect(int joint){
	if(joint == 1)
		PORTCbits._P5 = LOW;
	else
		PORTCbits._P4 = LOW;
}
/**
 * @brief sets the chip select high for the passed joint
 * @param joint 1 or 2 to deselect
 */
void slaveDeselect(int joint){
	if(joint == 1)
		PORTCbits._P5 = HIGH;
	else
		PORTCbits._P4 = HIGH;
}

/**
 * @brief holds processor until transmission has finished
 */
void waitForTransmissionEnd(){
	while(!(SPSR & BIT(SPIF))){
		//wait
	}
}
