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
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

uint8_t Button::check(){
	status = !HAL_GPIO_ReadPin(gpio_port, gpio_pin);
	if (status){
		everActivated = true;
	}
	return status;
}

uint8_t Button::isEverActivated(){
	return everActivated;
}

void Button::reset(){

}

uint8_t Button::getStatus(){
	return status;
}

