/** @brief Lab1 function definitions
 *
 * @file lab1.h
 *
 * This includes all the functions needed for Lab1.
 *
 * @authors cpbove@wpi.edu jlhonicker@wpi.edu dmmurray@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#ifndef LAB1_H_
#define LAB1_H_

// -- setup routines --

/**
 * @brief Configures Timer0 for Parts 2 and 7 of lab, prints header for logging
 */
void initPart2and7Timer();
/**
 * @brief Configures Timer1 for Part 4 of lab(signal generation), prints header for logging
 */
void initSignalGeneratorTimer();
/**
 * @brief Configures outputs for generating signals on pins
 */
void outputSetup();



// -- runtime functions --

/**
 * @brief Holds processor until button 7 is pressed
 *
 * @todo make more generic
 */
void waitForButton7();
/**
 * @brief Holds the processor until the right character is received from USART.
 *
 * @param c The character to wait for.
 */
void waitForChar(char c);
/**
 * @brief Streams timestamp, adcval, potmv, and potangle to PC w/ USART
 *
 * @param channel The ADC channel to read from
 */
void ADCToSerial(int channel);
/**
 * @brief Streams timestamp and ADC values to the serial port for 1 second
 *
 * @param channel The ADC channel to read
 */
void ADCToSerialPart7(int channel);
/**
 * @brief Configures postscaler in ISR based on inputted frequency.
 *
 * Only handles a few frequencies: 1,20,100
 *
 * @todo make more generic
 *
 * @param frequency The frequency to change to (1, 20, or 100)
 */
void setFrequencyForPostScale(int frequency);
/**
 * @brief Generates signals w/particular duty cycles set by pot
 *
 * Also reads buttons 5-7 to determine which frequency to output
 *
 * @param channel The ADC channel to read the pot from
 */
void signalGeneratorMain(int channel);


#endif /* LAB1_H_ */
