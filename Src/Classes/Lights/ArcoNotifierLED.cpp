/*
 * LEDUp.cpp
 *
 *  Created on: 05.02.2020
 *      Author: Igor
 */

#include <ArcoNotifierLED.h>

ArcoNotifierLED acro_notifier_led;


void ArcoNotifierLED::Init(){
	OFF();
	process_counter = 0;
}

void ArcoNotifierLED::Process(){
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

void ArcoNotifierLED::OFF(){
	HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_SET);
}

void ArcoNotifierLED::Toggle(){
	HAL_GPIO_TogglePin(LED_OUT_GPIO_Port, LED_OUT_Pin);
}

ArcoNotifierLED::ArcoNotifierLED() {
	// TODO Auto-generated constructor stub

}

ArcoNotifierLED::~ArcoNotifierLED() {
	// TODO Auto-generated destructor stub
}


