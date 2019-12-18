/*
 * Lights.h
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTS_H_
#define CLASSES_LIGHTS_H_

#include "Allshit.h"
#include "USBTask.h"

class Lights {
private:
	uint32_t lights_task_counter;
	uint32_t light_process_counter;
public:
	uint8_t stop_light;
	uint8_t stop_light_duration;
	void ws2812_init();
	void reset_data_buffer();
	void ws2812_set_color(int ledID, uint8_t r, uint8_t g, uint8_t b);
	void process();
	Lights();
	virtual ~Lights();
};

extern Lights lights;

#endif /* CLASSES_LIGHTS_H_ */
