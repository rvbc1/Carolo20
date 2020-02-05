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
}

void LEDUp::Process(){
	if(steering_manager.getRCmode() == ModeManager::MODE_ACRO) Toggle();
	else OFF();
	osDelay(1000);
}

void LEDUp::OFF(){
	HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_RESET);
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


