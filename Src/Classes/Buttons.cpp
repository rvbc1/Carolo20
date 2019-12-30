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

}
void Buttons::process(){
	//if(!start_parking_sent){
		if(HAL_GPIO_ReadPin(START_BUTTON_1_GPIO_Port, START_BUTTON_1_Pin) == GPIO_PIN_RESET){
			osDelay(40);
			if(HAL_GPIO_ReadPin(START_BUTTON_1_GPIO_Port, START_BUTTON_1_Pin) == GPIO_PIN_RESET){
				start_parking_USB = 1;
				left_indicator = !left_indicator;
			}
		}
//	}
//	else{
//		start_parking_USB = 0;
//	}
	//if(!start_obstacle_sent){
		if(HAL_GPIO_ReadPin(START_BUTTON_2_GPIO_Port, START_BUTTON_2_Pin) == GPIO_PIN_RESET){
			osDelay(40);
			if(HAL_GPIO_ReadPin(START_BUTTON_2_GPIO_Port, START_BUTTON_2_Pin) == GPIO_PIN_RESET){
				start_obstacle_USB = 1;
				right_indicator = !right_indicator;
			}
		}
//	}else{
//		start_obstacle_USB = 0;
//	}
	osDelay(5);
}


Buttons::Buttons() {
	// TODO Auto-generated constructor stub

}

Buttons::~Buttons() {
	// TODO Auto-generated destructor stub
}

