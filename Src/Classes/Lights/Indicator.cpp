/*
 * Indicator.cpp
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#include <Indicator.h>

uint16_t Indicator::proccess_counter = 0;

void Indicator::nextCycle(){
	proccess_counter++;
}

void Indicator::update(){
	if(proccess_counter > added){
		off();
	} else {
		on();
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

Indicator::Indicator(){
	proccess_counter = 0;
}

Indicator::~Indicator() {
	// TODO Auto-generated destructor stub
}

