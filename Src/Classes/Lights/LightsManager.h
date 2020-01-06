/*
 * Lights.h
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTSMANAGER_H_
#define CLASSES_LIGHTSMANAGER_H_

#include <AllTasks.h>
#include "USBTask.h"
#include "LEDStrip.h"
#include "Light.h"
#include "Indicator.h"

extern Light headlights;
extern Light tail_lights;
extern Light break_lights;

extern Indicator left_indicator_l;
extern Indicator right_indicator_l;

extern WS2812::Color high_beam_color;
extern WS2812::Color low_beam_color;
extern WS2812::Color tail_light_color;
extern WS2812::Color indicator_color;
extern WS2812::Color break_light_color;


class LightsManager {
private:
	uint32_t lights_task_counter;
	uint32_t light_process_counter;
public:
	uint8_t stop_light;
	uint8_t stop_light_duration;
	uint8_t high = false;

	void ws2812_init();
	void reset_data_buffer();
	void process();
	LightsManager();
	virtual ~LightsManager();
};

extern LightsManager lights_manager;

#endif /* CLASSES_LIGHTSMANAGER_H_ */
