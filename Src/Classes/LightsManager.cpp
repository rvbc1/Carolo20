/*
 * Lights.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#include <tim.h>
#include <string.h>


#include <LightsManager.h>


//#include <vector>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47
#define NUMBER_OF_LED_PCB			4
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
LED_Strip back_right(8);

Light headlights;
Light tail_lights;
Light break_lights;

Light left_indicator_l;
Light right_indicator_l;

WS2812::Color high_beam_color {255, 255, 255};
WS2812::Color low_beam_color {64, 64, 64};
WS2812::Color tail_light_color {20, 0 ,0};
WS2812::Color indicator_color {255, 100, 0};
WS2812::Color break_light_color {255, 0, 0};


uint16_t ws2812BitsBuffer[WS2812_BYTES_BUFFER_SIZE];

void LightsManager::ws2812_init() {

	front_left.setBuffer(&ws2812BitsBuffer[0]);
	front_right.setBuffer(&ws2812BitsBuffer[1*8*3*8]);
	back_left.setBuffer(&ws2812BitsBuffer[2*8*3*8]);
	back_right.setBuffer(&ws2812BitsBuffer[3*8*3*8]);




	headlights.setColor(high_beam_color);
	tail_lights.setColor(tail_light_color);
	break_lights.setColor(break_light_color);
	left_indicator_l.setColor(indicator_color);
	right_indicator_l.setColor(indicator_color);

	headlights.add(front_left.getLedAddress(5));
	headlights.add(front_left.getLedAddress(6));
	headlights.add(front_left.getLedAddress(7));

	headlights.add(front_right.getLedAddress(0));
	headlights.add(front_right.getLedAddress(1));
	headlights.add(front_right.getLedAddress(2));

	tail_lights.add(front_left.getLedAddress(0));
	tail_lights.add(front_right.getLedAddress(7));

	break_lights.add(front_left.getLedAddress(1));
	break_lights.add(front_right.getLedAddress(6));

	tail_lights.setActivated(true);
	break_lights.setActivated(true);


	MX_TIM4_Init();
	memset(ws2812BitsBuffer, 0, WS2812_BYTES_BUFFER_SIZE);

	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
	reset_data_buffer();
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);
	light_process_counter = 0;
	stop_light = false;
	stop_light_duration = 0;
}


void LightsManager::reset_data_buffer(){
	for(uint16_t i = 0; i < NUMBER_OF_LEDS * BITS_PER_LED; i++){
		ws2812BitsBuffer[i]=LOW_PWM_BIT_VALUE;
	}
}

void LightsManager::process(){
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);

	if(high){
		headlights.setColor(high_beam_color);
	} else {
		headlights.setColor(low_beam_color);
	}

	if(headlights.getActivated()){
		headlights.on();
	} else {
		headlights.off();
	}

	if(tail_lights.getActivated()){
		tail_lights.on();
	} else {
		tail_lights.off();
	}

	if(break_lights.getActivated()){
		break_lights.on();
	} else {
		break_lights.off();
	}



	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);

	osDelay(20);
}

LightsManager::LightsManager() {
	// TODO Auto-generated constructor stub

}

LightsManager::~LightsManager() {
	// TODO Auto-generated destructor stub
}

