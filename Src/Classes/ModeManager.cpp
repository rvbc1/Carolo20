/*
 * SteeringManager.cpp
 *
 *  Created on: Jan 11, 2020
 *      Author: rvbc-
 */

#include <ModeManager.h>
#include "cmsis_os.h"
#include "Futaba.h"
#include "ServoManager.h"
#include "Motor.h"
#include "main.h"
#include "iwdg.h"
#include "wwdg.h"
#include "Gyro.h"
#include "Odometry.h"
#include "AHRS.h"
#include "Tools.h"
#include "ButtonsManager.h"

ModeManager mode_manager;

static void StickCommandProccess(void) {
	if (futaba.Stick_Command[1]) // (   .)    (   .)
		motor.SetPassthroughState(true);
	else
		motor.SetPassthroughState(false);
	if (futaba.Stick_Command[0]) // (.   )    (.   )
		motor.setMaxVelocity(6000.f);
	else
		motor.setMaxVelocity(3500.f);

	static bool last_cmd = false;
	if (futaba.Stick_Command[4] != last_cmd) { // (.   )    (   .)   < - to nie sa cycki
		last_cmd = futaba.Stick_Command[4];
		gyro.StartCalibration();
		odometry.Reset(ahrs.attitude.values.yaw, motor.getDistance(),tools.GetMicros());
		odometry.SetCurrentPosition();
	}
}

void ModeManager::init(){

	futaba.ConfigureSmoothing(50.f, task_dt * 1e-3); /* Nyquist frequency - 1/2 Radio frequency * 0.9; 8CH - 9ms, 16CH - 18ms,*/

	MX_TIM13_Init();

	isUnlockDriveTimerRunning = false;

	TIM13->CNT = 1;

	osDelay(100);
}

void ModeManager::proccess(){
	futaba.ProcessSmoothing();

//	if(futaba.SwitchD == SWITCH_DOWN) service_mode = TESTING;
//	else 							  service_mode = CUP;

	if (futaba.Get_RCState() || futaba.SwitchA < SWITCH_DOWN) {
		rc_mode = DISARMED;
		drive_mode = DISABLE;

		if(isUnlockDriveTimerRunning && (service_mode == CUP)){
			breakUnlockDriveTimer();
		}

		if (futaba.Get_RCState() == 0)
			StickCommandProccess();
	} else if (futaba.SwitchA == SWITCH_DOWN) {
		if (service_mode == TESTING) drive_mode = ENABLE;

		if (futaba.SwitchB == SWITCH_UP) {
			rc_mode = MODE_ACRO;
			if(!isUnlockDriveTimerRunning && (service_mode == CUP)){
				drive_mode = DISABLE;
				startUnlockDriveTimer();
			}

		} else if (futaba.SwitchB == SWITCH_MIDDLE) {
			rc_mode = MODE_SEMI;
			drive_mode = ENABLE;
			if(isUnlockDriveTimerRunning && (service_mode == CUP)){
				breakUnlockDriveTimer();
			}

		} else if (futaba.SwitchB == SWITCH_DOWN) {
			rc_mode = MODE_AUTONOMOUS;
			drive_mode = ENABLE;
			if(isUnlockDriveTimerRunning && (service_mode == CUP)){
				breakUnlockDriveTimer();
			}

		}
	}

	osDelay(task_dt);
}

void ModeManager::ToggleServiceMode(){
	if(service_mode == TESTING) service_mode = TESTING;
	else service_mode = CUP;
}

void ModeManager::modeDelayTimIT(){
	//isModeDelayTimON = false;
	if(firstUnlockDriveTimerIT_flag){
		firstUnlockDriveTimerIT_flag = false;
	} else {
		drive_mode = ENABLE;
	}
}

void ModeManager::startUnlockDriveTimer(){
	isUnlockDriveTimerRunning = true;
	HAL_TIM_Base_Start_IT(&TIM_IDLE);
}

void ModeManager::breakUnlockDriveTimer(){
	isUnlockDriveTimerRunning = false;
	HAL_TIM_Base_Stop_IT(&TIM_IDLE);
	__HAL_TIM_SET_COUNTER(&TIM_IDLE, 0);
}

ModeManager::RC_MODE ModeManager::getRCmode(){
	return rc_mode;
}

ModeManager::DRIVE_MODE ModeManager::getDriveMode(){
	return drive_mode;
}

ModeManager::SERVICE_MODE ModeManager::getServiceMode(){
	return service_mode;
}

//UNUSED(rc_mode);

ModeManager::ModeManager() {
	// TODO Auto-generated constructor stub

}

ModeManager::~ModeManager() {
	// TODO Auto-generated destructor stub
}

