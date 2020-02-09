/*
 * Light.h
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTS_LIGHT_H_
#define CLASSES_LIGHTS_LIGHT_H_

#include "main.h"
#include <WS2812.h>

extern WS2812::Color off_light_color;

class Light {
public:
	void add(uint16_t* adress);
	void on();
	void off();

	void setColor(WS2812::Color color);
	void setColor(uint8_t r, uint8_t g, uint8_t b);

	void setActivated(uint8_t activated);
	uint8_t getActivated();
	uint8_t getAddedCount();

	Light();
	virtual ~Light();
protected:
	WS2812::Color color;

	uint16_t* adressTab[24];
	uint8_t added;

	uint8_t activated = false;
	void ws2812_set_color(uint16_t* address, uint8_t r, uint8_t g, uint8_t b);
};

#endif /* CLASSES_LIGHTS_LIGHT_H_ */
