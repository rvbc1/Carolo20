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
	Indicator();
	virtual ~Indicator();
private:
	uint16_t proccess_counter;
};

#endif /* CLASSES_LIGHTS_INDICATOR_H_ */
