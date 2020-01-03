/*
 * Buttons.cpp
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#include "Buttons.h"
#include "stm32f7xx_hal.h"

Buttons buttons;

uint8_t start_parking_USB = 0;
uint8_t start_obstacle_USB = 0;
uint8_t start_parking_sent = 0;
uint8_t start_obstacle_sent = 0;


void Buttons::Init(){
	start1_state_of_pressing = false;
	start2_state_of_pressing = false;
	screen1_state_of_pressing = false;
	screen2_state_of_pressing = false;
	screen3_state_of_pressing = false;
	any_button_was_pressed = false;

	button_one = new Button(START_BUTTON_1_O7_GPIO_Port, START_BUTTON_1_O7_Pin);

	button_two = new Button(START_BUTTON_2_O6_GPIO_Port, START_BUTTON_2_O6_Pin);

}
void Buttons::process(){
	button_one->check();
	if(button_one->getStatus()){
		left_indicator = true;
	} else {
		left_indicator = false;
	}

	button_two->check();
	if(button_two->getStatus()){
		right_indicator = true;
	} else {
		right_indicator = false;
	}
	osDelay(5);
}


Buttons::Buttons() {
	// TODO Auto-generated constructor stub

}

Buttons::~Buttons() {
	// TODO Auto-generated destructor stub
}

