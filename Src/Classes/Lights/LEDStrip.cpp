/*
 * LEDStrip.cpp
 *
 *  Created on: Jan 5, 2020
 *      Author: rvbc-
 */

#include <Lights/LEDStrip.h>

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

void LED_Strip::ws2812_set_color(int ledID, uint8_t r, uint8_t g, uint8_t b) {
	int i = ledID * BITS_PER_LED;
	uint8_t mask;
	mask = 0x80;
	while (mask) {
		buffer[i] = (mask & g) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		buffer[i] = (mask & r) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		buffer[i] = (mask & b) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
}

void LED_Strip::setHeadlights(uint16_t start_index, uint16_t end_index){
	headlights_start = start_index;
	headlights_end = end_index;
	have_headlights = true;
}

void LED_Strip::headlightsON(){
	if(have_headlights){
		for(int i = headlights_start; i < headlights_end; i++){
			ws2812_set_color(i, WHITE);
		}
	}
}

void LED_Strip::headlightsOFF(){
	if(have_headlights){
		for(int i = headlights_start; i < headlights_end; i++){
			ws2812_set_color(i, BLACK);
		}
	}
}

void LED_Strip::setTailLights(uint16_t start_index, uint16_t end_index){
	tail_lights_start = start_index;
	tail_lights_end = end_index;
	have_tail_lights = true;
}

void LED_Strip::tailLightsON(){
	if(have_tail_lights){
		for(int i = headlights_start; i < headlights_end; i++){
			ws2812_set_color(i, TAIL);
		}
	}
}

void LED_Strip::tailLightsOFF(){
	if(have_tail_lights){
		for(int i = headlights_start; i < headlights_end; i++){
			ws2812_set_color(i, BLACK);
		}
	}
}


LED_Strip::~LED_Strip() {
	// TODO Auto-generated destructor stub
}

