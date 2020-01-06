/*
 * Lights.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#include <Lights.h>
#include <tim.h>
#include <string.h>

#include <Lights/LEDStrip.h>
#include <Lights/Light.h>


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

#define FRONT_LEFT_START_INDEX 8
#define FRONT_LEFT_END_INDEX 15
#define FRONT_RIGHT_START_INDEX 0
#define FRONT_RIGHT_END_INDEX 7
#define BACK_LEFT_START_INDEX 16
#define BACK_LEFT_END_INDEX 23
#define BACK_RIGHT_START_INDEX 24
#define BACK_RIGHT_END_INDEX 31

#define SCALE 4

Lights lights;

//LED_Strip front_left(8);
//LED_Strip front_right(8);
//LED_Strip back_left(8);
//LED_Strip back_right(8);
//
//Light headlights;
//Light tail_lights;
//Light break_lights;
//
//Light left_indicator_l;
//Light right_indicator_l;

//WS2812::Color high_beam_color {255, 255, 255};
//WS2812::Color low_beam_color {128, 128, 128};
//WS2812::Color tail_light_color {20, 0 ,0};
//WS2812::Color indicator_color {255, 100, 0};
//WS2812::Color break_light_color {255, 100, 0};
//WS2812::Color break_light_color {255, 100, 0};


uint16_t ws2812BitsBuffer[WS2812_BYTES_BUFFER_SIZE];

void Lights::ws2812_init() {

//	front_left.setBuffer(&ws2812BitsBuffer[0]);
//	front_right.setBuffer(&ws2812BitsBuffer[1*8*3*8]);
//	back_left.setBuffer(&ws2812BitsBuffer[2*8*3*8]);
//	back_right.setBuffer(&ws2812BitsBuffer[3*8*3*8]);




//	headlights.setColor(high_beam_color);
//	tail_lights.setColor(tail_light_color);
//	break_lights.setColor(break_light_color);
//	left_indicator_l.setColor(indicator_color);
//	right_indicator_l.setColor(indicator_color);

	//headlights.add(front_left.getLedAddress(5));

//	front_left.setHeadlights(5,7);
//	front_right.setHeadlights(0,2);
//
//	back_left.setTailLights(7,7);
//	back_right.setTailLights(0,0);

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
//		if(left_indicator && right_indicator){
//
//			ws2812_set_color(FRONT_LEFT_START_INDEX + light_process_counter/SCALE, 255,100,0);
//
//			ws2812_set_color(FRONT_RIGHT_END_INDEX - light_process_counter/SCALE, 255,100,0);
//
//
//			ws2812_set_color(BACK_RIGHT_START_INDEX + light_process_counter/SCALE, 255,100,0);
//
//			ws2812_set_color(BACK_LEFT_END_INDEX - light_process_counter/SCALE, 255,100,0);
//
//			lights.stop_light = true;
//			lights.stop_light_duration = 0;
//
//		} else if (left_indicator){
//
//			ws2812_set_color(FRONT_LEFT_START_INDEX + light_process_counter/SCALE, 255,100,0);
//
//			ws2812_set_color(BACK_LEFT_END_INDEX - light_process_counter/SCALE, 255,100,0);
//		} else if (right_indicator){
//
//			ws2812_set_color(FRONT_RIGHT_END_INDEX - light_process_counter/SCALE, 255,100,0);
//
//			ws2812_set_color(BACK_RIGHT_START_INDEX + light_process_counter/SCALE, 255,100,0);
//		} else {
//			for(uint16_t i = 0; i < NUMBER_OF_LEDS * BITS_PER_LED; i++){
//				ws2812BitsBuffer[i]=LOW_PWM_BIT_VALUE;
//			}
//		}
//
//		if(stop_light){
//			ws2812_set_color(BACK_RIGHT_START_INDEX, 255,0,0);
//			ws2812_set_color(BACK_RIGHT_END_INDEX, 255,0,0);
//
//			ws2812_set_color(BACK_LEFT_START_INDEX, 255,0,0);
//			ws2812_set_color(BACK_LEFT_END_INDEX, 255,0,0);
//
//			//stop_light_duration++;
//			if(stop_light_duration > 4){
//				//stop_light = false;
//				stop_light_duration = 0;
//			}
//		}

//		if(stop_light){
//
//		}
//
//		if(road_lights){
//			front_left.headlightsON();
//			front_right.headlightsON();
//
//			back_left.tailLightsON();
//			back_right.tailLightsON();
//
//		}




//		ws2812_set_color(BACK_LEFT_START_INDEX, 20,0,0);
//		ws2812_set_color(BACK_RIGHT_END_INDEX, 20,0,0);
//		if(road_lights){
//		ws2812_set_color(FRONT_LEFT_START_INDEX, 255,255,255);
//		ws2812_set_color(FRONT_RIGHT_END_INDEX, 255,255,255);
//		ws2812_set_color(FRONT_LEFT_START_INDEX + 1, 255,255,255);
//		ws2812_set_color(FRONT_RIGHT_END_INDEX - 1, 255,255,255);
//		ws2812_set_color(FRONT_LEFT_START_INDEX + 2, 255,255,255);
//		ws2812_set_color(FRONT_RIGHT_END_INDEX - 2, 255,255,255);
//		}

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
		ws2812_set_color(FRONT_LEFT_START_INDEX + 1, 255,255,255);
		ws2812_set_color(FRONT_RIGHT_END_INDEX - 1, 255,255,255);
		ws2812_set_color(FRONT_LEFT_START_INDEX + 2, 255,255,255);
		ws2812_set_color(FRONT_RIGHT_END_INDEX - 2, 255,255,255);

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

