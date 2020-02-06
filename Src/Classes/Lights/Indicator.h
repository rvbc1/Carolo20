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
	void update();
	static void nextCycle();
	Indicator();
	virtual ~Indicator();

	static uint16_t proccess_counter;
private:
	//static uint16_t proccess_counter;
};

#endif /* CLASSES_LIGHTS_INDICATOR_H_ */
