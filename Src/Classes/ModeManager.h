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

#define UNLOCK_DRIVE_TIM htim13

class ModeManager {
public:
	enum RC_MODE{
		DISARMED = 0,
		MODE_AUTONOMOUS,
		MODE_SEMI,
		MODE_ACRO
	};

	enum DRIVE_MODE{
		DISABLE = 0,
		ENABLE
	};

	enum SERVICE_MODE{
		CUP = 0,
		TESTING

	};

	void init();
	void proccess();

	void ToggleServiceMode();
	void unlockDriveTimmerIT();

	RC_MODE getRCmode();
	DRIVE_MODE getDriveMode();
	SERVICE_MODE getServiceMode();

	ModeManager();
	virtual ~ModeManager();

private:
	RC_MODE rc_mode = DISARMED;
	DRIVE_MODE drive_mode = DISABLE;
	SERVICE_MODE service_mode = CUP;

	void startUnlockDriveTimer();
	void breakUnlockDriveTimer();

	uint8_t isUnlockDriveTimerRunning = false;
	uint8_t firstUnlockDriveTimerIT_flag = true;

	const uint32_t task_dt = 1u;
	const uint32_t init_task_dt = 100;
};

extern ModeManager mode_manager;

#endif /* TASKS_STEERINGMANAGER_H_ */
