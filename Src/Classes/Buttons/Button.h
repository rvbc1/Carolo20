/*
 * Button.h
 *
 *  Created on: 03.01.2020
 *      Author: Igor
 */

#ifndef CLASSES_BUTTON_H_
#define CLASSES_BUTTON_H_

#include "main.h"
#include "bitoperations.h"

#define AMOUNT_OF_FLAGS_PER_BUTTON 2
#define BUTTON_FLAGS_DATA_TYPE uint8_t

#define STATUS_BIT 0
#define EVER_ACTIVETED_BIT 1


class Button {
private:
	BUTTON_FLAGS_DATA_TYPE flags;
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_pin;
public:
	uint8_t check();
	uint8_t getStatus();
	uint8_t isEverActivated();
	BUTTON_FLAGS_DATA_TYPE getData();
	void reset();
	Button(GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
	virtual ~Button();
};

#endif /* CLASSES_BUTTON_H_ */
