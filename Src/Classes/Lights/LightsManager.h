/*
 * Lights.h
 *
 *  Created on: Jan 19, 2019
 *      Author: rvbc-
 */

#ifndef CLASSES_LIGHTSMANAGER_H_
#define CLASSES_LIGHTSMANAGER_H_

#include "USBLink.h"
#include "LEDStrip.h"
#include "Light.h"
#include "Indicator.h"
#include "ModeManager.h"
#include "Futaba.h"
#include "Motor.h"
#include "../../Tasks&Callbacks/AllTasks.h"

#define ACC_AVERAGE_NUM 300

#define MAX_LIGHTS_AMOUNT 10

extern Light headlights;
extern Light tail_lights;
extern Light break_lights;

extern Indicator left_indicator_front;
extern Indicator left_indicator_back;
extern Indicator right_indicator_front;
extern Indicator right_indicator_back;

extern WS2812::Color high_beam_color;
extern WS2812::Color low_beam_color;
extern WS2812::Color tail_light_color;
extern WS2812::Color indicator_color;
extern WS2812::Color break_light_color;


class LightsManager {
public:
	uint8_t high = false;

	uint16_t process_counter = 0;

	void hazard_warning_lights();

	void ws2812_init();
	void reset_data_buffer();
	void process();
	LightsManager();
	virtual ~LightsManager();
private:
	uint32_t lights_task_counter;
	uint32_t light_process_counter;

	float acceleration[ACC_AVERAGE_NUM];
	uint8_t acc_counter = 0;
	void checkRCmode();
	void breakLightProcess();

	uint8_t needAnyLightUpdate();
	void lightsUpdate();
	void indicatorsUpdate();

	uint16_t added_lights_count = 0;
	Light* all_lights [MAX_LIGHTS_AMOUNT];
	void addLight(Light* light);
};

extern LightsManager lights_manager;

#endif /* CLASSES_LIGHTSMANAGER_H_ */
