/*
 * Lights.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#include <tim.h>
#include <string.h>
#include "Encoder.h"


#include <LightsManager.h>


//#include <vector>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47
#define NUMBER_OF_LED_PCB			5
#define NUMBER_OF_LEDS_PER_PCB		8
#define NUMBER_OF_LEDS 				NUMBER_OF_LED_PCB * NUMBER_OF_LEDS_PER_PCB
#define BYTES_PER_LED 				3
#define BITS_PER_BYTE 				8
#define BITS_PER_LED 				BYTES_PER_LED * BITS_PER_BYTE
#define DATA_LOAD_BYTES 			81 	// FOR WS2812 AFTER DATA TRANSFER MUST BE ABOVE 50us LOW STATE ON DIN PIN
// 1BIT TAKE 1,25us
#define WS2812_BYTES_BUFFER_SIZE 	NUMBER_OF_LEDS * BITS_PER_LED + DATA_LOAD_BYTES


#define SCALE 4

LightsManager lights_manager;

LED_Strip front_left(8);
LED_Strip front_right(8);
LED_Strip back_left(8);
LED_Strip back_middle(8);
LED_Strip back_right(8);

Light headlights;
Light tail_lights;
Light break_lights;

Indicator left_indicator_front;
Indicator left_indicator_back;
Indicator right_indicator_front;
Indicator right_indicator_back;

WS2812::Color high_beam_color {255, 255, 255};
WS2812::Color low_beam_color {64, 64, 64};
WS2812::Color tail_light_color {20, 0 ,0};
//WS2812::Color indicator_color {255, 100, 0};
WS2812::Color indicator_color {255/5, 100/5, 0};
//WS2812::Color break_light_color {255, 0, 0};
WS2812::Color break_light_color {255/5, 0, 0};


uint16_t ws2812BitsBuffer[WS2812_BYTES_BUFFER_SIZE];

void LightsManager::ws2812_init() {
	added_lights_count= 0;

	front_right.setBuffer(&ws2812BitsBuffer[0*NUMBER_OF_LEDS_PER_PCB*NUMBER_OF_LEDS_PER_PCB*3]);
	front_left.setBuffer(&ws2812BitsBuffer[1*NUMBER_OF_LEDS_PER_PCB*NUMBER_OF_LEDS_PER_PCB*3]);
	back_left.setBuffer(&ws2812BitsBuffer[2*NUMBER_OF_LEDS_PER_PCB*NUMBER_OF_LEDS_PER_PCB*3]);
	back_middle.setBuffer(&ws2812BitsBuffer[3*NUMBER_OF_LEDS_PER_PCB*NUMBER_OF_LEDS_PER_PCB*3]);
	back_right.setBuffer(&ws2812BitsBuffer[4*NUMBER_OF_LEDS_PER_PCB*NUMBER_OF_LEDS_PER_PCB*3]);




	headlights.setColor(low_beam_color);
	tail_lights.setColor(tail_light_color);
	break_lights.setColor(break_light_color);
	left_indicator_front.setColor(indicator_color);
	right_indicator_front.setColor(indicator_color);

	left_indicator_back.setColor(indicator_color);
	right_indicator_back.setColor(indicator_color);

	headlights.add(front_right.getLedAddress(5));
	headlights.add(front_right.getLedAddress(6));
	headlights.add(front_right.getLedAddress(7));

	headlights.add(front_left.getLedAddress(0));
	headlights.add(front_left.getLedAddress(1));
	headlights.add(front_left.getLedAddress(2));

	tail_lights.add(back_left.getLedAddress(7));
	tail_lights.add(back_right.getLedAddress(0));

	break_lights.add(back_left.getLedAddress(5));
	break_lights.add(back_left.getLedAddress(6));

	break_lights.add(back_middle.getLedAddress(3));
	break_lights.add(back_middle.getLedAddress(4));

	break_lights.add(back_right.getLedAddress(1));
	break_lights.add(back_right.getLedAddress(2));

	right_indicator_front.add(front_right.getLedAddress(4));
	right_indicator_front.add(front_right.getLedAddress(3));
	right_indicator_front.add(front_right.getLedAddress(2));
	right_indicator_front.add(front_right.getLedAddress(1));
	right_indicator_front.add(front_right.getLedAddress(0));

	left_indicator_front.add(front_left.getLedAddress(3));
	left_indicator_front.add(front_left.getLedAddress(4));
	left_indicator_front.add(front_left.getLedAddress(5));
	left_indicator_front.add(front_left.getLedAddress(6));
	left_indicator_front.add(front_left.getLedAddress(7));

	left_indicator_back.add(back_left.getLedAddress(4));
	left_indicator_back.add(back_left.getLedAddress(3));
	left_indicator_back.add(back_left.getLedAddress(2));
	left_indicator_back.add(back_left.getLedAddress(1));
	left_indicator_back.add(back_left.getLedAddress(0));

	right_indicator_back.add(back_right.getLedAddress(3));
	right_indicator_back.add(back_right.getLedAddress(4));
	right_indicator_back.add(back_right.getLedAddress(5));
	right_indicator_back.add(back_right.getLedAddress(6));
	right_indicator_back.add(back_right.getLedAddress(7));


	headlights.setActivated(true);
	tail_lights.setActivated(true);
	break_lights.setActivated(true);


	left_indicator_front.setActivated(false);
	right_indicator_front.setActivated(false);

	left_indicator_back.setActivated(false);
	right_indicator_back.setActivated(false);

	addLight(&headlights);
	addLight(&tail_lights);
	addLight(&break_lights);
	addLight(&left_indicator_front);
	addLight(&right_indicator_front);
	addLight(&left_indicator_back);
	addLight(&right_indicator_back);


	MX_TIM4_Init();
	memset(ws2812BitsBuffer, 0, WS2812_BYTES_BUFFER_SIZE);

	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
	reset_data_buffer();
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);
	light_process_counter = 0;


	for(uint16_t i=0; i < ACC_AVERAGE_NUM; i++) acceleration[i] = 0;
}


void LightsManager::process(){
	checkRCmode();
	if(breakLightProcess()){
		break_counter++;
	} else {
		break_counter = 0;
	}

	if(process_counter % 10 == 0){
		if(break_counter >= 50){
			break_lights.setActivated(true);
		} else  break_lights.setActivated(false);
		lightsUpdate();
	}

	if(process_counter > 100){
		indicatorsUpdate();
		process_counter = 0;
	}


	process_counter++;
	osDelay(1);
}


void LightsManager::reset_data_buffer(){
	for(uint16_t i = 0; i < NUMBER_OF_LEDS * BITS_PER_LED; i++){
		ws2812BitsBuffer[i]=LOW_PWM_BIT_VALUE;
	}
}
uint8_t LightsManager::breakLightProcess(void){
	uint8_t return_value = false;
//	if((encoder.getAverageAcceleration() < -3 * encoder.getAverageVelocity() && encoder.getAverageVelocity() > 250) ||
//	   (encoder.getAverageAcceleration() >  3 * encoder.getAverageVelocity() && encoder.getAverageVelocity() < 250)){
//
//		if((encoder.getAverageAcceleration() < -3000.f) || (encoder.getAverageAcceleration() >  3000.f )) return_value = true; //break_lights.setActivated(true); 			// Break lights ON
//	}

	if((encoder.getAverageAcceleration() < -800 && encoder.getAverageVelocity() > 150) ||
	   (encoder.getAverageAcceleration() >  800 && encoder.getAverageVelocity() < 150)){ return_value = true;
	}

//	else{
//		break_lights.setActivated(false); 		    // Break lights OFF
//	}
	return return_value;
}

void LightsManager::checkRCmode(){
	if(mode_manager.getRCmode() == ModeManager::DISARMED){
		left_indicator_front.setActivated(true);
		right_indicator_front.setActivated(true);

		left_indicator_back.setActivated(true);
		right_indicator_back.setActivated(true);

	} else if (mode_manager.getRCmode() == ModeManager::MODE_ACRO){

//		if (futaba.SwitchC == SWITCH_UP) {
//			left_indicator_front.setActivated(false);
//			right_indicator_front.setActivated(true);
//
//			left_indicator_back.setActivated(false);
//			right_indicator_back.setActivated(true);
//		} else if(futaba.SwitchC == SWITCH_MIDDLE){
			left_indicator_front.setActivated(false);
			right_indicator_front.setActivated(false);

			left_indicator_back.setActivated(false);
			right_indicator_back.setActivated(false);
//		} else{
//			left_indicator_front.setActivated(true);
//			right_indicator_front.setActivated(false);
//
//			left_indicator_back.setActivated(true);
//			right_indicator_back.setActivated(false);
//		}
	} else {
		left_indicator_front.setActivated(setpoints_from_vision.left_inidcator);
		left_indicator_back.setActivated(setpoints_from_vision.left_inidcator);

		right_indicator_front.setActivated(setpoints_from_vision.right_inidcator);
		right_indicator_back.setActivated(setpoints_from_vision.right_inidcator);
	}
}

void LightsManager::addLight(Light* light){
	all_lights[added_lights_count] = light;
	added_lights_count++;
}

uint8_t LightsManager::needAnyLightUpdate(){
	uint8_t update = false;
	for(uint16_t i = 0; i < added_lights_count; i++){
		if(all_lights[i]->needUpdate()){
			update = true;
			break;
		}
	}
	return update;
}

void LightsManager::lightsUpdate(){
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);

	if(needAnyLightUpdate()){
		for(uint16_t i = 0 ; i < added_lights_count; i++)
			all_lights[i]->update();
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);
}

void LightsManager::indicatorsUpdate(){
	if(left_indicator_front.getActivated())
		left_indicator_front.nextCycle();


	if(right_indicator_front.getActivated())
		right_indicator_front.nextCycle();


	if(left_indicator_back.getActivated())
		left_indicator_back.nextCycle();


	if(right_indicator_back.getActivated())
		right_indicator_back.nextCycle();
}

LightsManager::LightsManager() {
	// TODO Auto-generated constructor stub

}

LightsManager::~LightsManager() {
	// TODO Auto-generated destructor stub
}
