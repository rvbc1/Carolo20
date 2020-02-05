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

class MotorManager {
public:
	void init();
	void process();
	MotorManager();
	virtual ~MotorManager();
};

extern MotorManager motor_manager;

#endif /* CLASSES_MOTOR_MOTORMANAGER_H_ */
