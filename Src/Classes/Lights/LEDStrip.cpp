/*
 * LEDStrip.cpp
 *
 *  Created on: Jan 5, 2020
 *      Author: rvbc-
 */

#include <LEDStrip.h>

#define BYTES_PER_LED 				3
#define BITS_PER_BYTE 				8
#define BITS_PER_LED 				BYTES_PER_LED * BITS_PER_BYTE


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

