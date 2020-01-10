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

	uint16_t* getLedAddress(uint16_t index);

	LED_Strip(uint16_t amount_of_leds);
	virtual ~LED_Strip();
private:


	uint16_t amount_of_leds;
	uint16_t* buffer;

};

#endif /* CLASSES_LIGHTS_LEDSTRIP_H_ */
