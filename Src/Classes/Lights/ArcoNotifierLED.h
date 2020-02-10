/*
 * LEDUp.h
 *
 *  Created on: 05.02.2020
 *      Author: Igor
 */

#ifndef CLASSES_LIGHTS_ARCONOTIFIERLED_H_
#define CLASSES_LIGHTS_ARCONOTIFIERLED_H_

#include "main.h"
#include "ModeManager.h"
#include "cmsis_os.h"

#include "../../Tasks&Callbacks/AllTasks.h"


class ArcoNotifierLED {
public:
	ArcoNotifierLED();
	void Init();
	void Process();
	virtual ~ArcoNotifierLED();
private:
	uint16_t process_counter;
	void OFF();
	void Toggle();
};

extern ArcoNotifierLED acro_notifier_led;

#endif /* CLASSES_LIGHTS_ARCONOTIFIERLED_H_ */

