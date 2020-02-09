/*
 * Indicator.cpp
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#include <Indicator.h>

void Indicator::nextCycle(){
	proccess_counter++;
	if(proccess_counter > added){
		off();
	}
}

void Indicator::on(){
	for(int i = 0; i < proccess_counter; i++){
		WS2812::setColor(adressTab[i], color);
	}
}

void Indicator::off(){
	proccess_counter = 0;
	for(int i = 0; i < added; i++){
		WS2812::setColor(adressTab[i], off_light_color);
	}
}

void Indicator::setActivated(uint8_t activated){
	if(getActivated() != activated){
		resetCounter();
		this->activated = activated;
	}
}

void Indicator::resetCounter(){
	proccess_counter = 0;
}

Indicator::Indicator(){
	resetCounter();
}

Indicator::~Indicator() {
	// TODO Auto-generated destructor stub
}
