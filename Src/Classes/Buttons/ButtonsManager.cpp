/*
 * Buttons.cpp
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#include <ButtonsManager.h>
#include "stm32f7xx_hal.h"
#include "bitoperations.h"

//#include <vector>
//
//std::vector


ButtonsManager buttons_manager;

uint8_t start_parking_USB = 0;
uint8_t start_obstacle_USB = 0;
uint8_t start_parking_sent = 0;
uint8_t start_obstacle_sent = 0;




void ButtonsManager::Init(){
	ever1 = false;
	ever2 = false;
	first1 = false;
	first2 = false;
	start1_state_of_pressing = false;
	start2_state_of_pressing = false;
	screen1_state_of_pressing = false;
	screen2_state_of_pressing = false;
	screen3_state_of_pressing = false;
	any_button_was_pressed = false;

	button_one = new Button(START_BUTTON_1_O7_GPIO_Port, START_BUTTON_1_O7_Pin);

	button_two = new Button(START_BUTTON_2_O6_GPIO_Port, START_BUTTON_2_O6_Pin);

}

uint8_t ButtonsManager::getState(){
	uint8_t value = 0;
	changeBit(value, 0, button_one->getStatus());
	changeBit(value, 1, ever1);
	changeBit(value, 2, button_two->getStatus());
	changeBit(value, 3, ever2);
	changeBit(value, 4, first1);
	changeBit(value, 5, first2);
	return value;
}

void ButtonsManager::check(){
	button_one->check();
	button_two->check();
}
void ButtonsManager::process(){
	check();

	if(button_one->getStatus()){
		ever1 = true;
		if(first2 == 0)
			first1 = true;
	}

	if(button_two->getStatus()){
		ever2 = true;
		if(first1 == 0)
			first2 = true;
	}

	//	if(button_one->getStatus()){
	//		left_indicator = true;
	//	} else {
	//		left_indicator = false;
	//	}
	//
	//	if(button_two->getStatus()){
	//		right_indicator = true;
	//	} else {
	//		right_indicator = false;
	//	}
	osDelay(5);
}

void ButtonsManager::resert_buttons(){
	ever1 = false;
	ever2 = false;
	first1 = false;
	first2 = false;
}


ButtonsManager::ButtonsManager() {
	// TODO Auto-generated constructor stub

}

ButtonsManager::~ButtonsManager() {
	// TODO Auto-generated destructor stub
}

