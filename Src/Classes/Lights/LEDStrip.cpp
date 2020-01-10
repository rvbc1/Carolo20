/*
 * LEDStrip.cpp
 *
 *  Created on: Jan 5, 2020
 *      Author: rvbc-
 */

#include <LEDStrip.h>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47
#define NUMBER_OF_LED_PCB			4
#define NUMBER_OF_LEDS_PER_PCB		8
#define NUMBER_OF_LEDS 				NUMBER_OF_LED_PCB * NUMBER_OF_LEDS_PER_PCB
#define BYTES_PER_LED 				3
#define BITS_PER_BYTE 				8
#define BITS_PER_LED 				BYTES_PER_LED * BITS_PER_BYTE

#define WHITE 						255,255,255
#define BLACK						0,0,0
#define TAIL						20,0,0

LED_Strip::LED_Strip(uint16_t amount_of_leds) {
	this->amount_of_leds = amount_of_leds;
}


void LED_Strip::setBuffer(uint16_t* buff){
	this->buffer = buff;
}

uint16_t* LED_Strip::getLedAddress(uint16_t index){
	return &buffer[index * BITS_PER_LED];
}


LED_Strip::~LED_Strip() {
	// TODO Auto-generated destructor stub
}

