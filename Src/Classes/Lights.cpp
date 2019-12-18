/*
 * Lights.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#include <Lights.h>
#include <tim.h>
#include <string.h>

#define HIGH_PWM_BIT_VALUE 			91
#define LOW_PWM_BIT_VALUE 			47
#define NUMBER_OF_LED_PCB			3
#define NUMBER_OF_LEDS_PER_PCB		8
#define NUMBER_OF_LEDS 				NUMBER_OF_LED_PCB * NUMBER_OF_LEDS_PER_PCB
#define BYTES_PER_LED 				3
#define BITS_PER_BYTE 				8
#define BITS_PER_LED 				BYTES_PER_LED * BITS_PER_BYTE
#define DATA_LOAD_BYTES 			81 	// FOR WS2812 AFTER DATA TRANSFER MUST BE ABOVE 50us LOW STATE ON DIN PIN
// 1BIT TAKE 1,25us
#define WS2812_BYTES_BUFFER_SIZE 	NUMBER_OF_LEDS * BITS_PER_LED + DATA_LOAD_BYTES

#define FRONT_LEFT_START_INDEX 4
#define FRONT_LEFT_END_INDEX 7
#define FRONT_RIGHT_START_INDEX 0
#define FRONT_RIGHT_END_INDEX 3
#define BACK_LEFT_START_INDEX 8
#define BACK_LEFT_END_INDEX 15
#define BACK_RIGHT_START_INDEX 16
#define BACK_RIGHT_END_INDEX 23

#define SCALE 4

Lights lights;

uint16_t ws2812BitsBuffer[WS2812_BYTES_BUFFER_SIZE];

void Lights::ws2812_init() {
	MX_TIM4_Init();
	memset(ws2812BitsBuffer, 0, WS2812_BYTES_BUFFER_SIZE);

	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
	reset_data_buffer();
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);
	light_process_counter = 0;
	stop_light = false;
	stop_light_duration = 0;
}

void Lights::ws2812_set_color(int ledID, uint8_t r, uint8_t g, uint8_t b) {
	int i = ledID * BITS_PER_LED;
	uint8_t mask;
	mask = 0x80;
	while (mask) {
		ws2812BitsBuffer[i] = (mask & g) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		ws2812BitsBuffer[i] = (mask & r) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
	mask = 0x80;
	while (mask) {
		ws2812BitsBuffer[i] = (mask & b) ? HIGH_PWM_BIT_VALUE : LOW_PWM_BIT_VALUE;
		mask >>= 1;
		i++;
	}
}

void Lights::reset_data_buffer(){
	for(uint16_t i = 0; i < NUMBER_OF_LEDS * BITS_PER_LED; i++){
		ws2812BitsBuffer[i]=LOW_PWM_BIT_VALUE;
	}
}

void Lights::process(){
	if(light_process_counter < NUMBER_OF_LEDS_PER_PCB*SCALE){
		HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
		if(left_indicator && right_indicator){

			ws2812_set_color(FRONT_LEFT_START_INDEX + light_process_counter/SCALE/2, 255,100,0);

			ws2812_set_color(FRONT_RIGHT_END_INDEX - light_process_counter/SCALE/2, 255,100,0);


			ws2812_set_color(BACK_RIGHT_START_INDEX + light_process_counter/SCALE, 255,100,0);

			ws2812_set_color(BACK_LEFT_END_INDEX - light_process_counter/SCALE, 255,100,0);

			lights.stop_light = true;
			lights.stop_light_duration = 0;

		} else if (left_indicator){

			ws2812_set_color(FRONT_LEFT_START_INDEX + light_process_counter/SCALE/2, 255,100,0);

			ws2812_set_color(BACK_LEFT_END_INDEX - light_process_counter/SCALE, 255,100,0);
		} else if (right_indicator){

			ws2812_set_color(FRONT_RIGHT_END_INDEX - light_process_counter/SCALE/2, 255,100,0);

			ws2812_set_color(BACK_RIGHT_START_INDEX + light_process_counter/SCALE, 255,100,0);
		} else {
			for(uint16_t i = 0; i < NUMBER_OF_LEDS * BITS_PER_LED; i++){
				ws2812BitsBuffer[i]=LOW_PWM_BIT_VALUE;
			}
		}

		if(stop_light){
			ws2812_set_color(BACK_RIGHT_START_INDEX, 255,0,0);
			ws2812_set_color(BACK_RIGHT_END_INDEX, 255,0,0);

			ws2812_set_color(BACK_LEFT_START_INDEX, 255,0,0);
			ws2812_set_color(BACK_LEFT_END_INDEX, 255,0,0);

			//stop_light_duration++;
			if(stop_light_duration > 4){
				//stop_light = false;
				stop_light_duration = 0;
			}
		}

		ws2812_set_color(FRONT_LEFT_START_INDEX, 255,255,255);
		ws2812_set_color(FRONT_RIGHT_END_INDEX, 255,255,255);
		ws2812_set_color(BACK_LEFT_START_INDEX, 20,0,0);
		ws2812_set_color(BACK_RIGHT_END_INDEX, 20,0,0);
		//ws2812_set_color(FRONT_LEFT_START_INDEX + 1, 255,255,255);
		//ws2812_set_color(FRONT_RIGHT_END_INDEX - 1, 255,255,255);
		//ws2812_set_color(FRONT_LEFT_START_INDEX + 2, 255,255,255);
		//ws2812_set_color(FRONT_RIGHT_END_INDEX - 2, 255,255,255);

		HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);
		light_process_counter++;
	} else {
		HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);

		reset_data_buffer();

		if(stop_light){
			ws2812_set_color(BACK_RIGHT_START_INDEX, 255,0,0);
			ws2812_set_color(BACK_RIGHT_END_INDEX, 255,0,0);

			ws2812_set_color(BACK_LEFT_START_INDEX, 255,0,0);
			ws2812_set_color(BACK_LEFT_END_INDEX, 255,0,0);

			//stop_light_duration++;
			if(stop_light_duration > 4){
				//stop_light = false;
				stop_light_duration = 0;
			}
		}

		ws2812_set_color(FRONT_LEFT_START_INDEX, 255,255,255);
		ws2812_set_color(FRONT_RIGHT_END_INDEX, 255,255,255);
		ws2812_set_color(BACK_LEFT_START_INDEX, 20,0,0);
		ws2812_set_color(BACK_RIGHT_END_INDEX, 20,0,0);
//		ws2812_set_color(FRONT_LEFT_START_INDEX + 1, 255,255,255);
//		ws2812_set_color(FRONT_RIGHT_END_INDEX - 1, 255,255,255);
//		ws2812_set_color(FRONT_LEFT_START_INDEX + 2, 255,255,255);
//		ws2812_set_color(FRONT_RIGHT_END_INDEX - 2, 255,255,255);

		HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812BitsBuffer, WS2812_BYTES_BUFFER_SIZE);

		light_process_counter = 0;
	}
	osDelay(20);
}

Lights::Lights() {
	// TODO Auto-generated constructor stub

}

Lights::~Lights() {
	// TODO Auto-generated destructor stub
}

