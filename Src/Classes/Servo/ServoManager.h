/*
 * ServoManager.h
 *
 *  Created on: Jan 11, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_SERVOMANAGER_H_
#define CLASSES_SERVOMANAGER_H_

#include "main.h"
#include "Servo.h"

class ServoManager {
public:
	void init();

	void arm();
	void disarm();

	void process();

	void setAngle(int16_t front, int16_t back);

	ServoManager();
	virtual ~ServoManager();

private:
	Servo* servo_back;
	Servo* servo_front;

	void checkDriveMode();
	void checkRCmode();

	static const uint32_t task_dt = 1u;
};

extern ServoManager servo_manager;

#endif /* CLASSES_SERVOMANAGER_H_ */
