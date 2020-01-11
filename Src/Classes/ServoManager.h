/*
 * ServoManager.h
 *
 *  Created on: Jan 11, 2020
 *      Author: Marek
 */

#ifndef CLASSES_SERVOMANAGER_H_
#define CLASSES_SERVOMANAGER_H_

#include "main.h"
#include "Servo.h"

class ServoManager {
private:
	Servo* servo_back;
	Servo* servo_front;
public:
	void init();

	void arm();
	void disarm();

	void setAngle(uint16_t front, uint16_t back);

	ServoManager();
	virtual ~ServoManager();
};

extern ServoManager servo_manager;

#endif /* CLASSES_SERVOMANAGER_H_ */
