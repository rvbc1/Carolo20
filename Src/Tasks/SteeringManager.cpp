/*
 * SteeringManager.cpp
 *
 *  Created on: Jan 11, 2020
 *      Author: rvbc-s
 */

#include <SteeringManager.h>

#include "cmsis_os.h"
#include "Futaba.h"
#include "ServoManager.h"
#include "Motor.h"
#include "main.h"
#include "iwdg.h"
#include "wwdg.h"

SteeringManager steering_manager;

void SteeringManager::init(){

	futaba.ConfigureSmoothing(50.f, task_dt * 1e-3); /* Nyquist frequency - 1/2 Radio frequency * 0.9; 8CH - 9ms, 16CH - 18ms,*/

	servo_manager.init();

	osDelay(100);
}

void SteeringManager::proccess(){
	futaba.ProcessSmoothing();

	if (futaba.Get_RCState() || futaba.SwitchA < SWITCH_DOWN) {
		rc_mode = DISARMED;

		//if (futaba.Get_RCState() == 0)
		//	StickCommandProccess();
	} else if (futaba.SwitchA == SWITCH_DOWN) {
		if (futaba.SwitchB == SWITCH_UP) {
			rc_mode = MODE_ACRO;

		} else if (futaba.SwitchB == SWITCH_MIDDLE) {
			rc_mode = MODE_SEMI;

		} else if (futaba.SwitchB == SWITCH_DOWN) {
			rc_mode = MODE_AUTONOMOUS;

		}
	}


	//TODO - Find best suited place for watchdog refreshes
	static uint8_t watchdog_init_done = 0;
	if (watchdog_init_done) {
		HAL_WWDG_Refresh(&hwwdg);
		HAL_IWDG_Refresh(&hiwdg);
	} else if(HAL_GetTick() > 500) {
		MX_IWDG_Init();
		MX_WWDG_Init();
		watchdog_init_done = 1;
	}

	osDelay(task_dt);
}

SteeringManager::RC_MODE SteeringManager::getRCmode(){
	return rc_mode;
}

//UNUSED(rc_mode);

SteeringManager::SteeringManager() {
	// TODO Auto-generated constructor stub

}

SteeringManager::~SteeringManager() {
	// TODO Auto-generated destructor stub
}

