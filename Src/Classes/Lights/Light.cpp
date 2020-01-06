/*
 * Light.cpp
 *
 *  Created on: Jan 6, 2020
 *      Author: Marek
 */

#include <Lights/Light.h>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47
#define NUMBER_OF_LED_PCB			4
#define NUMBER_OF_LEDS_PER_PCB		8
#define NUMBER_OF_LEDS 				NUMBER_OF_LED_PCB * NUMBER_OF_LEDS_PER_PCB
#define BYTES_PER_LED 				3
#define BITS_PER_BYTE 				8
#define BITS_PER_LED 				BYTES_PER_LED * BITS_PER_BYTE

void Light::ws2812_set_color(uint16_t* address, uint8_t r, uint8_t g, uint8_t b){
	int i = 0;
	uint8_t mask;
	uint16_t* buffer = address;
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

void Light::setColor(WS2812::Color color){
	this->color = color;
}


void Light::setColor(uint8_t r, uint8_t g, uint8_t b){
	color.r = r;
	color.g = g;
	color.b = b;
}

void Light::add(uint16_t* adress){
	adressTab[added] = adress;
	added++;
}

void Light::on(){
	for(int i = 0; i < added; i++){
		WS2812::setColor(adressTab[i], color);
	}
}

void Light::off(){
	for(int i = 0; i < added; i++){
		ws2812_set_color(adressTab[i], 0, 0, 0);
	}
}

Light::Light() {
	// TODO Auto-generated constructor stub
	added = 0;
}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

