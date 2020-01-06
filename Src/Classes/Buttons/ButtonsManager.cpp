/*
 * Buttons.cpp
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#include <ButtonsManager.h>
#include "stm32f7xx_hal.h"


ButtonsManager buttons_manager;

uint8_t start_parking_USB = 0;
uint8_t start_obstacle_USB = 0;
uint8_t start_parking_sent = 0;
uint8_t start_obstacle_sent = 0;


void ButtonsManager::Init(){
	start1_state_of_pressing = false;
	start2_state_of_pressing = false;
	screen1_state_of_pressing = false;
	screen2_state_of_pressing = false;
	screen3_state_of_pressing = false;
	any_button_was_pressed = false;

	button_one = new Button(START_BUTTON_1_O7_GPIO_Port, START_BUTTON_1_O7_Pin);

	button_two = new Button(START_BUTTON_2_O6_GPIO_Port, START_BUTTON_2_O6_Pin);

}

void ButtonsManager::check(){
	button_one->check();
	button_two->check();
}
void ButtonsManager::process(){
	check();

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


ButtonsManager::ButtonsManager() {
	// TODO Auto-generated constructor stub

}

ButtonsManager::~ButtonsManager() {
	// TODO Auto-generated destructor stub
}

