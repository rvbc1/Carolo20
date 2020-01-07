
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
	uint8_t status1, status2;
	button_one->check();
	button_two->check();
	status1 = button_one->getStatus();
	status2 = button_two->getStatus();
	updateFlag(status1, status2);
}
void ButtonsManager::updateFlag(uint8_t s1, uint8_t s2){
	activatedFirst(s1,s2);
	activatedEver(s1,s2);
	active(s1,s2);
}


void ButtonsManager::activatedFirst(uint8_t s1, uint8_t s2){
	if (s1 < s2){//pierwszy byl 2 przycisk
		buttonFlag = changeBit(buttonFlag,0, 5);
		buttonFlag = changeBit(buttonFlag,1, 4);
	}
	else if ((s1 > s2) || ((s1==s2) && (s1 == 1))){ //kiedy pierwszy byl 1 przycisk lub oba byly jednoczesnie
		buttonFlag = changeBit(buttonFlag,1, 5);
		buttonFlag = changeBit(buttonFlag,0, 4);
	}
	else ; //zaden nie zostal wlaczony
}

void ButtonsManager::activatedEver(uint8_t s1, uint8_t s2){
	uint8_t prev1, prev2;
	prev1 = getBit(buttonFlag, 3);
	prev2 = getBit(buttonFlag, 2);

	if ((prev1 == 0) && (s1 == 1)){
		buttonFlag = changeBit(buttonFlag,1, 3);
	}
	if ((prev2 == 0) && (s2 == 1)){
		buttonFlag = changeBit(buttonFlag,1, 2);
	}
}
void ButtonsManager::active(uint8_t s1, uint8_t s2){
	if (s1 == 1) buttonFlag = changeBit(buttonFlag,1, 1);
	else         buttonFlag = changeBit(buttonFlag,0, 1);
	if (s2 == 1) buttonFlag = changeBit(buttonFlag,1, 0);
	else         buttonFlag = changeBit(buttonFlag,0, 0);
}

void ButtonsManager::process(){
	check();

	if(button_one->getStatus()){
		left_indicator = true;
	} else {
		left_indicator = false;
	}

	if(button_two->getStatus()){
		right_indicator = true;
	} else {
		right_indicator = false;
	}
	osDelay(5);
}


ButtonsManager::ButtonsManager() {
	// TODO Auto-generated constructor stub

}

ButtonsManager::~ButtonsManager() {
	// TODO Auto-generated destructor stub
}


