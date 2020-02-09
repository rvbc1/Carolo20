/*
 * MotorManager.h
 *
 *  Created on: Jan 12, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_MOTOR_MOTORMANAGER_H_
#define CLASSES_MOTOR_MOTORMANAGER_H_

#include "main.h"
#include "Motor.h"
#include "ModeManager.h"
#include "cmsis_os.h"

#define ACRO_MAX_VELOCITY 300.f
#define SEMI_MAX_VELOCITY 6000.f
#define AUTONOMOUS_MAX_VELOCITY 6000.f


class MotorManager {
private:
	void DriveModeCheck();
	void RCModeCheck();
	void setMaxVelocity(ModeManager::RC_MODE mode);
public:
	void init();
	void process();
	MotorManager();
	virtual ~MotorManager();
};

extern MotorManager motor_manager;

#endif /* CLASSES_MOTOR_MOTORMANAGER_H_ */
