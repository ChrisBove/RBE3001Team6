/** @brief ADC setup function definitions
 *
 * @file ADC.C
 *
 * This has everything needed to configure the ADC for
 * 10 bit operation and reading the values.
 *
 *
 * @author cpbove@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

#include "RBELib/RBELib.h"
#include "include/definitions.h" // our definitions

/**
 * @var adch
 * for storing the adch register value after an interrupt
 *
 * @var adcl
 * for storing the adch register value after an interrupt
 *
 * @var readNewChannel
 * used to make sure we read the new channel after we switch channels
 */
volatile unsigned short adch;
volatile unsigned short adcl;
volatile unsigned short readNewChannel;

/**
 * @brief ISR for updating ADC readings
 * Runs when ADC reading completes based on configured ADC settings
 *
 * @param ADC_vect Interrupt vector for ADC on chip
 *
 */
ISR(ADC_vect) {
	adcl = ADCL; // set globals equal to register
	adch = ADCH;
	readNewChannel++; // increase read count
}

/**
 * @brief Initializes the ADC and makes one channel active.
 *
 * Enables ADC to use interrupts
 *
 * @param channel The ADC channel to initialize
 *
 */
void initADC(int channel){
	adch = 0; // init global
	adcl = 0;
	readNewChannel = 0;
	ADCSRA |= BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0); // set division for sampling at 128kHz
	ADMUX |= BIT(REFS0); //set voltage ref to AVCC=5V
	//ADMUX |= BIT(ADLAR); //using 10-bit conversion, used to left-justify for 8 bit conversion
	changeADC(channel);

	ADCSRA |= BIT(ADATE); //set auto trigger on
	ADCSRB &= ~(BIT(ADTS2)|BIT(ADTS1)|BIT(ADTS0));//set to free running

	ADCSRA |= BIT(ADEN); // set enable bit
	ADCSRA |= BIT(ADIE); //set ADC Interrupt on
	sei(); //set global interrupts

	ADCSRA |= BIT(ADSC); // start conversions
}

/**
 * @brief Disables ADC functionality and clears any saved values (globals).
 *
 * This still needs to be tested.
 *
 * @param channel  The ADC channel to disable.
 *
 * @todo Create the corresponding function to clear the last ADC
 * calculation register and disconnect the input to the ADC if desired.
 */
void clearADC(int channel){
	ADCSRA &= ~BIT(ADEN); //disable ADC
	ADCL = 0x00; // clear ADCL register Needed??
	ADCH = 0x00; // clear ADCH register Needed??
	adch = 0; // clear global
	adcl = 0;
	//ADCSRA &= ~ADIF; // clear ADIF
	//channel = ~BIT(ADEN); // disable this channel

}

/**
 * @brief Get the analog value from the configured channel
 *
 * @param channel  The ADC channel to run a conversion on.
 * @return adcVal The 10 bit value returned by the ADC conversion.
 *
 */
unsigned short getADC(int channel){
	static int lastChannel = 0;
	// if the passed channel has changed since last time, change ADC channel
	if(lastChannel != channel){
		changeADC(channel); // change channel
		lastChannel = channel;
		readNewChannel = 0; // set this global to 0
		// make sure we get latest reading from new channel
		while(readNewChannel < 2){
			//wait for ISR to run twice
			// todo make this more intelligent
		}
	}
	// grab latest registry values
	cli(); // stop global interrupts
	unsigned short temp_adch = adch;
	unsigned short temp_adcl = adcl;
	sei(); // set global interrupts

	// parse the 2 registers to get 1 10bit value
	unsigned int adc_val;
	adc_val = temp_adch & 0x3; // gets last 2 bits
	adc_val = adc_val << 8; //move those 2 bits over 8 to make room
	adc_val |= temp_adcl; // fill empty 8 bits with adcl bits
	return adc_val;
}

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 */
void changeADC(int channel){
	ADMUX &= 0xF8; // zero out current mux channel 1111 1000
	ADMUX |= channel; // set mux channel TODO make this more efficient
}
