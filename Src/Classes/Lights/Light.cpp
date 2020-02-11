/*
 * Light.cpp
 *
 *  Created on: Jan 6, 2020
 *      Author: rvbc-
 */

#include <Light.h>

WS2812::Color off_light_color {0, 0, 0};

void Light::setColor(WS2812::Color color){
	this->color = color;
}


void Light::setColor(uint8_t r, uint8_t g, uint8_t b){
	color.r = r;
	color.g = g;
	color.b = b;
}

void Light::add(uint16_t* adress){
	adressTab[added] = adress;
	added++;
}

void Light::setActivated(uint8_t activated){
	this->activated = activated;
	need_update = true;
}

uint8_t Light::getActivated(){
	return activated;
}

void Light::update(){
	if(getActivated()){
		on();
	} else {
		off();
	}
	need_update = false;
}

uint8_t Light::needUpdate(){
	return need_update;
}

void Light::on(){
	for(int i = 0; i < added; i++){
		WS2812::setColor(adressTab[i], color);
	}
}

void Light::off(){
	for(int i = 0; i < added; i++){
		WS2812::setColor(adressTab[i], off_light_color);
	}
}

uint8_t Light::getAddedCount(){
	return added;
}


Light::Light() {
	// TODO Auto-generated constructor stub
	added = 0;
	need_update = false;
}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

