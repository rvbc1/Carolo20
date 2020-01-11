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

class SteeringManager {
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
	SteeringManager();
	virtual ~SteeringManager();
private:
	const uint32_t task_dt = 1u;
	RC_MODE rc_mode = DISARMED;

};

extern SteeringManager steering_manager;

#endif /* TASKS_STEERINGMANAGER_H_ */
