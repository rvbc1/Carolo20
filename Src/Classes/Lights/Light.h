/*
 * Light.h
 *
 *  Created on: Jan 6, 2020
 *      Author: Marek
 */

#ifndef CLASSES_LIGHTS_LIGHT_H_
#define CLASSES_LIGHTS_LIGHT_H_

#include "main.h"

class Light {
public:
	void add(uint16_t* adress);
	void on();
	void off();

	void setColor(uint8_t r, uint8_t g, uint8_t b);

	Light();
	virtual ~Light();
private:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	uint16_t* adressTab[24];
	uint8_t added;
	void ws2812_set_color(uint16_t* address, uint8_t r, uint8_t g, uint8_t b);
};

#endif /* CLASSES_LIGHTS_LIGHT_H_ */
