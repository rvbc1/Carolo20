
/*
 * Buttons.cpp
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#include <ButtonsManager.h>


ButtonsManager buttons_manager;

void ButtonsManager::Init(){

	button_one = new Button(START_BUTTON_1_O7_GPIO_Port, START_BUTTON_1_O7_Pin);
	button_two = new Button(START_BUTTON_2_O6_GPIO_Port, START_BUTTON_2_O6_Pin);
	addButton(button_one);
	addButton(button_two);


}
void ButtonsManager::addButton(Button *button){
	if(amount_of_added_buttons < MAX_BUTTONS_AMOUNT){
		all_buttons[amount_of_added_buttons] = button;
		amount_of_added_buttons++;
	}
}

ALL_BUTTONS_FLAGS_DATA_TYPE ButtonsManager::getData(){
	ALL_BUTTONS_FLAGS_DATA_TYPE all_buttons_flags = 0;
	for(int i = 0; i < amount_of_added_buttons; i++){
		all_buttons_flags += all_buttons[i]->getData() << (i * AMOUNT_OF_FLAGS_PER_BUTTON);
	}
	all_buttons_flags += first_clicked_button_flag;
	return all_buttons_flags;
}

void ButtonsManager::check(){
	for(int i = 0; i < amount_of_added_buttons; i++){
		if(all_buttons[i]->check() && (first_clicked_button_flag == 0)){
			setBit(first_clicked_button_flag, i + amount_of_added_buttons * AMOUNT_OF_FLAGS_PER_BUTTON);
		}
	}
}


void ButtonsManager::reset(){
	for(int i = 0; i < amount_of_added_buttons; i++){
		all_buttons[i]->reset();
	}
	first_clicked_button_flag = 0;
}


void ButtonsManager::process(){
	check();
	osDelay(5);
}


ButtonsManager::ButtonsManager() {
	// TODO Auto-generated constructor stub

}

ButtonsManager::~ButtonsManager() {
	// TODO Auto-generated destructor stub
}



