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


#define SERVICE_MAX_VELOCITY 6000.f

#define ACRO_MAX_VELOCITY 800.f
#define SEMI_MAX_VELOCITY 6000.f
#define AUTONOMOUS_MAX_VELOCITY 6000.f



class MotorManager {
public:
	void init();
	void process();
	MotorManager();
	virtual ~MotorManager();

private:
	void DriveModeCheck();
	void RCModeCheck();
	void setMaxVelocity();

	static const uint32_t task_dt = 1u;
};

extern MotorManager motor_manager;

#endif /* CLASSES_MOTOR_MOTORMANAGER_H_ */
