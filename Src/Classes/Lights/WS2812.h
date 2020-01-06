/*
 * WS2812.h
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTS_WS2812_H_
#define CLASSES_LIGHTS_WS2812_H_

#include "main.h"

class WS2812 {
public:
	struct Color{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
	static void setColor(uint16_t* address, Color color);
	WS2812();
	virtual ~WS2812();
};

#endif /* CLASSES_LIGHTS_WS2812_H_ */
