/*
 * LEDStrip.h
 *
 *  Created on: Jan 5, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTS_LEDSTRIP_H_
#define CLASSES_LIGHTS_LEDSTRIP_H_

#include "main.h"

class LED_Strip {
public:
	void setBuffer(uint16_t* buff);

	void on();
	void off();

	void setHeadlights(uint16_t start_index, uint16_t end_index);
	void setTailLights(uint16_t start_index, uint16_t end_index);


	void headlightsON();
	void headlightsOFF();

	void tailLightsON();
	void tailLightsOFF();

	uint16_t* getLedAddress(uint16_t index);

	LED_Strip(uint16_t amount_of_leds);
	virtual ~LED_Strip();
private:
	uint8_t have_headlights = false;
	uint8_t have_tail_lights = false;

	uint16_t amount_of_leds;
	uint16_t* buffer;

	uint16_t headlights_start;
	uint16_t headlights_end;

	uint16_t tail_lights_start;
	uint16_t tail_lights_end;

	void ws2812_set_color(int ledID, uint8_t r, uint8_t g, uint8_t b);
};

#endif /* CLASSES_LIGHTS_LEDSTRIP_H_ */
