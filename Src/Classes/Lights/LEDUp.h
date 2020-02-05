/*
 * LEDUp.h
 *
 *  Created on: 05.02.2020
 *      Author: Igor
 */

#ifndef CLASSES_LIGHTS_LEDUP_H_
#define CLASSES_LIGHTS_LEDUP_H_

#include "main.h"
#include "ModeManager.h"
#include "cmsis_os.h"
#include "AllTasks.h"


class LEDUp {
public:
	LEDUp();
	void Init();
	void Process();
	virtual ~LEDUp();
private:
	void OFF();
	void Toggle();
};

extern LEDUp ledUp;

#endif /* CLASSES_LIGHTS_LEDUP_H_ */

