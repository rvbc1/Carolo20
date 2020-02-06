/*
 * SteeringManager.h
 *
 *  Created on: Jan 11, 2020
 *      Author: rvbc-
 */

#ifndef TASKS_STEERINGMANAGER_H_
#define TASKS_STEERINGMANAGER_H_

#include "main.h"
#include "LightsManager.h"

#define TIM_IDLE htim13

class ModeManager {
public:
	enum RC_MODE{
		DISARMED = 0,
		MODE_AUTONOMOUS,
		MODE_SEMI,
		MODE_ACRO
	};

	void init();
	void proccess();
	RC_MODE getRCmode();

	void modeDelayTimIT();
	ModeManager();
	virtual ~ModeManager();
private:
	const uint32_t task_dt = 1u;
	RC_MODE rc_mode = DISARMED;

	void idleStart();
	void idleReset();
	bool isModeDelayTimON = false;

};

extern ModeManager steering_manager;

#endif /* TASKS_STEERINGMANAGER_H_ */
