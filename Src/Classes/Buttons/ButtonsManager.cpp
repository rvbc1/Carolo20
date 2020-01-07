
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
	status1 = button_one->check();
	status2 = button_two->check();
	updateFlag(status1, status2);
}
void ButtonsManager::updateFlag(uint8_t s1, uint8_t s2){
	activatedEver();
	activatedFirst(s1,s2);
	active(s1,s2);
}


void ButtonsManager::activatedFirst(uint8_t s1, uint8_t s2){
	if      (s1 && buttonFlag < 16) buttonFlag = changeBit(buttonFlag,s1, BUTTON1_BIT_FIRST);
	else if (s2 && buttonFlag < 16) buttonFlag = changeBit(buttonFlag,s2, BUTTON2_BIT_FIRST);
}

void ButtonsManager::reset_activatedFirstFlag(){
	buttonFlag = changeBit(buttonFlag, false, BUTTON1_BIT_FIRST);
	buttonFlag = changeBit(buttonFlag, false, BUTTON2_BIT_FIRST);
}

void ButtonsManager::active(uint8_t s1, uint8_t s2){
	buttonFlag = changeBit(buttonFlag, s1, BUTTON1_BIT_ACTIVE);
	buttonFlag = changeBit(buttonFlag, s2, BUTTON2_BIT_ACTIVE);
}

void ButtonsManager::activatedEver(){
	if (!getBit(buttonFlag, BUTTON1_BIT_EVER) && button_one->isEverActivated()) buttonFlag = changeBit(buttonFlag, true, BUTTON1_BIT_EVER);
	if (!getBit(buttonFlag, BUTTON2_BIT_EVER) && button_two->isEverActivated()) buttonFlag = changeBit(buttonFlag, true, BUTTON2_BIT_EVER);
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


