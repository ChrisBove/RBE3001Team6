/** @brief Lab1 functions
 *
 * @file lab1.c
 *
 * This includes all the functions needed for Lab1.
 *
 * @authors cpbove@wpi.edu jlhonicker@wpi.edu dmmurray@wpi.edu
 * @date 28-Jan-2016
 * @version 1.0
 */

//includes
#include "RBELib/RBELib.h"
#include "include/definitions.h" // our definitions
#include "include/lab1.h"


//globals
/**
 * @var timer0Count
 * counts time in 0.01s or 1/225 seconds (depends on active lab part)
 * @var timer2Count
 * counts time for outputting signals
 * @var timer2CountSub
 * another counter within the Timer2 ISR
 * @var postScale
 * for dividing the ISR output more
 */
volatile unsigned long timer0Count;
volatile unsigned long timer2Count;
volatile unsigned timer2CountSub;
unsigned postScale;

// ===== SETUPS =====
/**
 * @brief Configures Timer0 for Parts 2 and 7 of lab, prints header for logging
 */
void initPart2and7Timer() {
	// print the logging header
	printf("Time(s),ADC Val,Pot Voltage,Angle(deg)\n\r"); // PART 2
	//printf("Time(s),ADC Val\n\r"); // part 7

	TCCR0A |= BIT(WGM01);// set to CTC
	TCCR0A |= BIT(COM0A1); // Configure timer 1 for CTC mode

	TCCR0B |= BIT(CS02) | BIT(CS00); // prescale by 1024 = 18kHz
	OCR0A = 179; // 79 to get 225Hz // part 2 value:179; // divide by 180 -1  to get 100 Hz count

	timer0Count = 0; // initialize timercount
	TIMSK0 |= (1 << OCIE0A); // Enable CTC interrupt
	sei(); // Enable global interrupts
}

/**
 * @brief Configures Timer1 for Part 4 of lab(signal generation), prints header for logging
 */
void initSignalGeneratorTimer() {
		// Prints the duty cycle, frequency, state, and pot value to the serial port
		printf("Duty Cycle (percentage),Frequency,State,Pot Val\n\r");

		TCCR2A |= BIT(WGM21); // set to CTC
		TCCR2A |= BIT(COM2A1); // Configure timer 1 for CTC mode

		TCCR2B |= BIT(CS22); //div by 64 to get 288000
		OCR2A = 70; //count 70 to get ~ 4096 ///////10017.3913 Hz ~ 10kHz
		setFrequencyForPostScale(1); //OCR1A = 35999; // divide by 180 -1  to get 100 Hz count

		timer2Count = 0; // initialize timercount
		timer2CountSub = 0;
		TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
		sei(); // Enable global interrupts
}

/**
 * @brief Configures outputs for generating signals on pins
 */
void outputSetup(){
	DDRBbits._P3 = OUTPUT;
	DDRBbits._P2 = OUTPUT;
}



// ======= RUNTIME FUNCTIONS ======
/**
 * @brief Holds processor until button 7 is pressed
 *
 * @todo make more generic
 */
void waitForButton7() {
	//lab1 Part 7 initialization
	printf("Push button 7\n\r");
	while (PINDbits._P7 == 1) {
		//wait
	}
}
/**
 * @brief Holds the processor until the right character is received from USART.
 *
 * @param c The character to wait for.
 */
void waitForChar(char c){
	while(getCharDebug()!= c){
		// do nothing
	}
}
/**
 * @brief Streams timestamp, adcval, potmv, and potangle to PC w/ USART
 * Transmits timestamps, ADC value in counts, pot output in millivolts, and pot angle
 * in degrees over the serial port to the screen in the Terminal.
 *
 * @param channel The ADC channel to read from
 */
void ADCToSerial(int channel) {
	cli(); //stop interrupts
	unsigned int time = timer0Count;
	sei(); // start interrupts
	unsigned int adcval = getADC(channel);
	unsigned int potmv = (5000.0 * adcval)/1023.0; //calc pot millivolts todo use defined voltage
	unsigned int potangle = (300.0 * adcval)/1023.0; // calc pot angle todo use defined angle

	printf("%.2f,%u,%u,%u\n\r", time/100.0, adcval, potmv, potangle);
}
/**
 * @brief Streams timestamp and ADC values to the serial port for 1 second
 * Transmits timestamps, ADC value in counts
 * in degrees over the serial port to the screen in the Terminal.
 *
 * @param channel The ADC channel to read
 */
void ADCToSerialPart7(int channel) {
	cli(); //stop interrupts
	unsigned int time = timer0Count;
	sei(); // start interrupts
	unsigned int adcval = getADC(channel);
	static int lastTimerCount = -1; // store the last timer count, inits to something invalid
	// print data for 225 runs (1 second of logging)
	if(timer0Count <225 && lastTimerCount !=time){
		printf("%.4f,%u\n\r", time*0.00444, adcval);
		lastTimerCount = time;
	}

}

/**
 * @brief Configures postscaler in ISR based on inputted frequency.
 *
 * Only handles a few frequencies: 1,20,100
 *
 * @todo make more generic
 *
 * @param Frequency The frequency to change to (1, 20, or 100)
 */
void setFrequencyForPostScale(int Frequency){
	cli(); //stop interrupts
	switch(Frequency){
	case 225:
		postScale = 44 - 1; //count 44 to get about 225Hz
		break;
	case 100:
		postScale = 1 - 1;
		break;
	case 20:
		postScale = 5 - 1; // about 2kHz
		break;
	case 1:
		postScale = 100 - 1; // about 100Hz
		break;
	default:
		printf("Not a valid frequency!!\n\r");
		break;
	}
	sei(); //start interupts
}

/**
 * @brief Generates signals w/particular duty cycles set by pot
 *
 * Also reads buttons 5-7 to determine which frequency to output
 *
 * @param channel The ADC channel to read the pot from
 */
void signalGeneratorMain(int channel) {
	unsigned int adcval = getADC(channel);
	float dutyCycle = (100.0 * adcval) / 1023.0; // calc dudy cycle
	//printf("dutyCycle %f\n\r",dutyCycle);

	// storing button states
	static unsigned whichButton = 5;
	static unsigned b5LastState = 2;
	static unsigned b6LastState = 1;
	static unsigned b7LastState = 1;

	static unsigned freq = 0; // store frequency
	// read buttons
	unsigned char b5 = PINDbits._P5;
	unsigned char b6 = PINDbits._P6;
	unsigned char b7 = PINDbits._P7;
	//switch frequency based on which button was last pressed
	if (b5LastState != b5) {
		whichButton = 5;
		b5LastState = b5;
		setFrequencyForPostScale(1);
		freq = 1;
	} else if (b6LastState != b6) {
		whichButton = 6;
		b6LastState = b6;
		setFrequencyForPostScale(20);
		freq = 20;
	} else if (b7LastState != b7) {
		whichButton = 7;
		b7LastState = b7;
		setFrequencyForPostScale(100);
		freq = 100;
	}

	// while we haven't counted up for the full duty cycle high time, keep pin high
	while (timer2Count < dutyCycle) {
		PORTBbits._P3 = 1;
		// Prints the duty cycle, frequency, state, and pot value to the serial port
		printf("%.2f,%u,%u,%u\n\r", dutyCycle, freq, 1, adcval);
	}
	//now we've hit the point where the pin needs to go low
	// so set the pin low until we are done with this cycle
	while ((timer2Count > dutyCycle) && (timer2Count < 100)) {
		PORTBbits._P3 = 0;
		// Prints the duty cycle, frequency, state, and pot value to the serial port
		printf("%.2f,%u,%u,%u\n\r", dutyCycle, freq, 0, adcval);
	}
	if (timer2Count > 99) { // after period completes for one high low cycle, reset
		timer2Count = 0;  // reset timers
	}

}
/**
 * @brief generates triangle waves through DAC
 */
void triangleSignalGengerator(){
	int x=0;
	while(timer2Count<4095){
		setDAC(0,x);
		setDAC(1,(4095-x));
		x++;
	}
	while(timer2Count>4095){
		setDAC(0,(8191-x));
		setDAC(1,(x-4095));
		x++;
		if(x>8195){
			x=0;
			timer2Count = 0;
		}
	}
}


