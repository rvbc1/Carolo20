/*
 * LEDUp.cpp
 *
 *  Created on: 05.02.2020
 *      Author: Igor
 */

#include <LEDUp.h>

LEDUp ledUp;


void LEDUp::Init(){
	OFF();
	process_counter = 0;
}

void LEDUp::Process(){
	if(mode_manager.getRCmode() == ModeManager::MODE_ACRO){
		process_counter++;
		if(process_counter >= 10){
			process_counter = 0;
			Toggle();
		}
	}
	else {
		process_counter = 10;
		OFF();
	}
	osDelay(100);
}

void LEDUp::OFF(){
	HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_SET);
}

void LEDUp::Toggle(){
	HAL_GPIO_TogglePin(LED_OUT_GPIO_Port, LED_OUT_Pin);
}

LEDUp::LEDUp() {
	// TODO Auto-generated constructor stub

}

LEDUp::~LEDUp() {
	// TODO Auto-generated destructor stub
}


