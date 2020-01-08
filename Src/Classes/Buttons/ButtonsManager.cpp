
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
	addButton(button_one);
	addButton(button_two);

}
void ButtonsManager::addButton(Button *b){
	all_buttons[button_number] = b;
	button_number++;
}

uint8_t ButtonsManager::getData(){
	uint8_t return_value = 0;
	for(uint8_t NumberOfBit = 0; NumberOfBit < MAX_BUTTON_AMOUNT; NumberOfBit++){
			uint8_t status = all_buttons[NumberOfBit]->check();
			return_value |= ( status << NumberOfBit );
	}
	return return_value;
}

void ButtonsManager::check(){
	uint8_t status = getData();
	updateFlag(status);
}
void ButtonsManager::updateFlag(uint8_t status){
	active(status);
	activatedEver(status);
	activatedFirst(status);

}

void ButtonsManager::active(uint8_t status){
	for(uint8_t NumberOfBit = 0; NumberOfBit < MAX_BUTTON_AMOUNT; NumberOfBit++){
		buttonFlag = changeBit(buttonFlag , NumberOfBit, getBit(status, NumberOfBit));
	}
}

void ButtonsManager::activatedEver(uint8_t status){
	buttonFlag |= (status << MAX_BUTTON_AMOUNT);
}

void ButtonsManager::activatedFirst(uint8_t status){
	if(isEverActivated_reset){
		for(uint8_t NumberOfBit = 0 ; NumberOfBit < MAX_BUTTON_AMOUNT; NumberOfBit++){
			if(getBit(status, NumberOfBit)){
				buttonFlag = changeBit(buttonFlag, NumberOfBit + (2 * MAX_BUTTON_AMOUNT), true ); // third package of flags starts after active flags and ever activated flags
				isEverActivated_reset = false;
				break;
			}
		}
	}
}

void ButtonsManager::reset_activatedFirstFlag(){
	for(uint8_t NumberOfBit = 0 ; NumberOfBit < MAX_BUTTON_AMOUNT; NumberOfBit++){
		buttonFlag = changeBit(buttonFlag, NumberOfBit + (2 * MAX_BUTTON_AMOUNT), false );
	}
	isEverActivated_reset = true;
}

void ButtonsManager::reset_activatedEverFlag(){
	for(uint8_t NumberOfBit = 0 ; NumberOfBit < MAX_BUTTON_AMOUNT; NumberOfBit++){
		buttonFlag = changeBit(buttonFlag, (NumberOfBit + MAX_BUTTON_AMOUNT), false );
	}
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


