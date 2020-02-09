/*
 * Indicator.h
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTS_INDICATOR_H_
#define CLASSES_LIGHTS_INDICATOR_H_

#include "Light.h"

class Indicator : public Light {
public:
	void on();
	void off();
	void nextCycle();
	void setActivated(uint8_t activated);
	Indicator();
	virtual ~Indicator();
private:
	void resetCounter();
	uint16_t proccess_counter;
};

#endif /* CLASSES_LIGHTS_INDICATOR_H_ */
