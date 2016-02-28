/** @brief peripheral library for encoder and accel
 *
 * @file Periph.c
 *
 *
 * @author cpbove@wpi.edu with help from Joe St. Germain
 * @date 15-02-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/encoder.h"


/**
 * 
 * @def ENC_CNTR
 * hex value for addressing the CNTR register on the encoder
 * @def ENC_CLR_CMD
 * hex value for clearing the encoder count
 * @def ENC_RD_CMD
 * hex value command for reading the encoder count
 */
#define ENC_CNTR 0x20
#define ENC_CLR_CMD 0x00
#define ENC_RD_CMD 0x40

/**
 * @brief Find the acceleration in the given axis (X, Y, Z).
 * @param  axis The axis that you want to get the measurement of.
 * @return gVal Value of  acceleration.
 *
 */
signed int getAccel(int axis){
	// these are used to store the vref for a first run calibration
	static unsigned int Vref = 0;
	static BOOL isFirstTime = TRUE; // flag for if this is the first run
	
	//if first run, get and store the Vref, which won't change
	if (isFirstTime) {
		unsigned int firstResult = 0;

		PORTDbits._P7 = OFF; // enable accelerometer chip select

		// request and get the axis value
		spiTransceive(0b00000110); // transmit the command to read axis value
		firstResult = spiTransceive((3 << 6)); // get ADC channel 3, Vref

		// mask and shift bits from 2 receives to assemble 12 bit value
		firstResult = (firstResult & 0x0F) << 8;
		firstResult += spiTransceive(0x00);

		PORTDbits._P7 = ON; // disable accelerometer chip select

		Vref = (firstResult); // store Vref
		isFirstTime = FALSE; // note that first time is over
	}

	unsigned int result = 0; // for storing read axis value

	PORTDbits._P7 = OFF; // enable accelerometer chip select

	// request and get the axis value
	spiTransceive(0b00000110); // transmit the command to read axis value
	result = spiTransceive((axis << 6)); // get axis channel

	// mask and shift bits from 2 receives to assemble 2 byte value
	result = (result & 0x0F) << 8;
	result += spiTransceive(0x00);

	PORTDbits._P7 = ON;// disable accelerometer chip select

	// subtract vref from result to get non-offset value
	return ((result) - Vref);
}

/**
 * @brief Read an IR sensor and calculate the distance of the block.
 * @param  chan The port that the IR sensor is on.
 * @return value The distance the block is from the sensor.
 *
 * @todo Make a function that is able to get the ADC value of the IR sensor.
 */
int IRDist(int chan){
	return 0;
}

/**
 * @brief Initialize the encoders with the desired settings.
 * @param chan Channel to initialize (change: Joint 1 or 2)
 *
 * @note leaves the encoder in Byte 4, Quad X1 default state
 *
 */
void encInit(int chan){
	// based on channel, set pin as output and put it HIGH
	if(chan == 1){
		DDRCbits._P5 = OUTPUT; // set chip select to output
		PORTCbits._P5 = HIGH; // pull high to disable chip
	}
	else {
		DDRCbits._P4 = OUTPUT;// set chip select to output
		PORTCbits._P4 = HIGH; // pull high to disable chip
	}

	slaveSelect(chan); // select that slave
	spiTransceive(ENC_CLR_CMD | ENC_CNTR); // clear and reset commands
	slaveDeselect(chan); // deselect the slave

}

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear. (change: Joint 1 or 2)
 *
 */
void resetEncCount(int chan){
	singleByteWrite(CLR_CNTR,0x00,chan); //clear the count register
}

/**
 * @brief Finds the current count of one of the encoders.
 * @param  chan Channel of the encoder (change: Joint 1 or 2)
 * @return count The current count of the encoder.
 *
 */
signed long encCount(int chan){
	//4 byte variables for storing packets
	signed long temp1 = 0;
	signed long temp2 = 0;
	signed long temp3 = 0;
	signed long temp4 = 0;

	//final value to return
	signed long encData = 0;

	//select the encoder SPI Slave
	slaveDeselect(chan);
	slaveSelect(chan);

	// request a read from the encoder
	spiTransceive(ENC_RD_CMD | ENC_CNTR);

	//save result in 4 bytes
	temp1 = spiTransceive(0x00);
	temp2 = spiTransceive(0x00);
	temp3 = spiTransceive(0x00);
	temp4 = spiTransceive(0x00);

	// shift data by order received (MSB first)
	encData = (temp1 << 24) | (temp2 << 16) | (temp3 << 8) | (temp4);

	slaveDeselect(chan); // deselect the slave

	return encData;
}
