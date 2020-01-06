/*
 * WS2812.cpp
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#include <WS2812.h>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47

void WS2812::setColor(uint16_t* address, Color color){
	int i = 0;
	uint8_t mask;
	uint16_t* buffer = address;
	mask = 0x80;

	while (mask) {
		buffer[i] = (mask & color.g) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		buffer[i] = (mask & color.r) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		buffer[i] = (mask & color.b) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
}

WS2812::WS2812() {
	// TODO Auto-generated constructor stub

}

WS2812::~WS2812() {
	// TODO Auto-generated destructor stub
}

