/** @brief arm library
 *
 * @file SPI.c
 *
 *
 * @author cpbove@wpi.edu
 * @date 30-Jan-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/definitions.h"

/**
 * @brief Initializes the SPI bus for communication with all of your
 * SPI devices.
 */
void initSPI(){
	DDRBbits._P7 = OUTPUT; // sclk
	DDRBbits._P5 = OUTPUT; // MOSI
	DDRBbits._P4 = OUTPUT; // Slave Select is output
	DDRBbits._P6 = INPUT; //MISO
	DDRDbits._P4 = OUTPUT; // chip select for DAC
	DDRDbits._P7 = OUTPUT; // chip select for accel

	PRR &= ~BIT(PRSPI); // enable SPI module by writing 0 to Power Reduction Reg
	SPCR =  BIT(SPE) | BIT(MSTR) | BIT(1); //set enable and master bits fclk/8
	//SPSR = BIT(SPI2X); //clock rate fck/2
	PORTDbits._P4 = HIGH; // disable chip select
	PORTDbits._P7 = HIGH; // disable accel chip select

}

/**
 * @brief Send and receive a byte out of the MOSI line.
 *
 * Please note that even if you do not want to receive any data back
 * from a SPI device, the SPI standard requires you still receive something
 * back even if it is junk data.
 *
 * @param data The byte to send down the SPI bus.
 * @return value The byte shifted in during transmit
 *
 */
unsigned char spiTransceive(BYTE data){
	SPDR = data;
	// wait for transmission to finish
	while(!(SPSR & BIT(SPIF))){
		//wait
	}
	unsigned char spsr = SPSR; //read these to clear interrupts
	unsigned char spdr = SPDR;

	return spdr; // return sent data
}
