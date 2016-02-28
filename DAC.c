/** @brief DAC library
 *
 * @file DAC.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 30-Jan-2016
 * @version 1.0
 */

#include"RBELib/RBELib.h"

/**
 * @brief Set the DAC to the given value on the chosen channel.
 * @param  DACn The channel (0-3) that you want to set.
 * @param SPIVal The value you want to set it to.
 *
 */
void setDAC(int DACn, int SPIVal){
	BYTE commandAndAddress = 0x30; // command: write to and update DAC register
	commandAndAddress |= DACn; // address to DAC A-D

	unsigned long temp;
	// limit SPIVal between 0 and 4095
	if((SPIVal >= 0) && (SPIVal <=4095))
		temp = SPIVal;
	else if (SPIVal >=4095)
		temp = 4095;
	else
		temp  = 0;

	// parse the temp DAC value into 2 bytes
	temp = temp << 4; // move values over by 4 bits
	//split that into 2 bytes
	BYTE byte1 = (temp & 0xFF00) >> 8;// take first 8 bits and shift into LSB
	BYTE byte2 = (temp & 0x00FF); // mask last 8 bits for byte 2

	PORTDbits._P4 = LOW; // turn on chip select

	// send our information
	spiTransceive(commandAndAddress); //send first byte
	spiTransceive(byte1); // send second byte
	spiTransceive(byte2); // send third byte

	// toggle slave select then turn it off
	PORTDbits._P4 = HIGH; // SS
	PORTDbits._P4 = LOW; // SS
	PORTDbits._P4 = HIGH; // SS

}
