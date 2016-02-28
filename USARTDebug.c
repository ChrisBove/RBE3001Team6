/*
 * USARTDebug.c
 *
 *  Created on: Jan 23, 2016
 *      Author: Chris Bove
 */

#include"RBELib/RBELib.h"
#include"include/definitions.h"

/**
 * @brief Initializes USART1 as a print terminal to the PC. This function
 * must check the incoming baudrate against the valid baudrates
 * from the data-sheet. If the baudrate is invalid, then the
 * DEFAULT_BAUD constant must be used instead.
 *
 * @param baudrate The desired baudrate to set for USART1.
 *
 */
void debugUSARTInit(unsigned long baudrate){
	//USART1_TX_vect = OUTPUT;
	//set the USART Control and Status Registers
	UCSR1A = 0b00100000;
	UCSR1B = 0b00000000;
	UCSR1C = 0b00000110;
	UCSR1B = ~BIT(U2X1); //disable double speed operation
	UCSR1B = BIT(RXEN1); //enable receive
	UCSR1B |= BIT(TXEN1); //enable transmit
	UBRR1H = (F_CLOCK / (16 * baudrate) - 1) >> 8;
	UBRR1L = (F_CLOCK / (16 * baudrate) - 1);
}

/**
 * @brief Sends one byte to the USART1 Tx pin (Transmits one byte).
 *
 * @param byteToSend The byte that is to be transmitted through USART1.
 *
 */
void putCharDebug(char byteToSend){
	loop_until_bit_is_set(UCSR1A, UDRE1); // wait...
	UDR1 = byteToSend; // write char to PC
}

/**
 * @brief Recieves one byte of data from the serial port (i.e. from the PC).
 *
 * @return byteReceived Character that was received on the USART.
 *
 */
unsigned char getCharDebug(void){
	loop_until_bit_is_set(UCSR1A, RXC1); // wait until data is received
	char c = UDR1; // read char from serial
	return c;
}
