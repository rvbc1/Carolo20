/*
 * Button.cpp
 *
 *  Created on: 03.01.2020
 *      Author: Igor
 */

#include <Button.h>

Button::Button(GPIO_TypeDef* gpio_port, uint16_t gpio_pin) {
	this->gpio_port = gpio_port;
	this->gpio_pin = gpio_pin;
	reset();
}

uint8_t Button::check(){
	uint8_t status = !HAL_GPIO_ReadPin(gpio_port, gpio_pin);
	changeBit(flags, STATUS_BIT, status);
	if (status){
		changeBit(flags, EVER_ACTIVETED_BIT, true);
	}
	return status;
}


BUTTON_FLAGS_DATA_TYPE Button::getData(){
	return flags;
}

uint8_t Button::getStatus(){
	return getBit(flags, STATUS_BIT);;
}

uint8_t Button::isEverActivated(){
	return getBit(flags, EVER_ACTIVETED_BIT);
}


void Button::reset(){
	flags = 0;
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}


