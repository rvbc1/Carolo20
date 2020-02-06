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

ModeManager steering_manager;

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

	servo_manager.init();

	osDelay(100);
}

void ModeManager::proccess(){
	futaba.ProcessSmoothing();

	if (futaba.Get_RCState() || futaba.SwitchA < SWITCH_DOWN) {
		rc_mode = DISARMED;

		if (futaba.Get_RCState() == DISARMED)
			StickCommandProccess();
	} else if (futaba.SwitchA == SWITCH_DOWN) {
		if (futaba.SwitchB == SWITCH_UP) {
			if(rc_mode == MODE_AUTONOMOUS){
				if ( !isModeDelayTimON ) idleStart();
			}
			else rc_mode = MODE_ACRO;

		} else if (futaba.SwitchB == SWITCH_MIDDLE) {
			if(isModeDelayTimON) idleReset();
			rc_mode = MODE_SEMI;

		} else if (futaba.SwitchB == SWITCH_DOWN) {
			if(isModeDelayTimON) idleReset();
			rc_mode = MODE_AUTONOMOUS;

		}
	}

	servo_manager.process();


	//TODO - Find best suited place for watchdog refreshes

	osDelay(task_dt);
}

void ModeManager::modeDelayTimIT(){
	idleReset();
	rc_mode = MODE_ACRO;
}

void ModeManager::idleStart(){
	isModeDelayTimON = true;
	HAL_TIM_Base_Start_IT(&TIM_IDLE);
}

void ModeManager::idleReset(){
	isModeDelayTimON = false;
	HAL_TIM_Base_Stop_IT(&TIM_IDLE);
	__HAL_TIM_SET_COUNTER(&TIM_IDLE, 0);
}

ModeManager::RC_MODE ModeManager::getRCmode(){
	return rc_mode;
}

//UNUSED(rc_mode);

ModeManager::ModeManager() {
	// TODO Auto-generated constructor stub

}

ModeManager::~ModeManager() {
	// TODO Auto-generated destructor stub
}

