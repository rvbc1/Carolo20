/*
 * Servo.h
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#ifndef SERVO_H_
#define SERVO_H_

#define MIN_PULSE_WIDTH 			1000
#define MAX_PULSE_WIDTH 			1900
#define DEFAULT_MIN_ANGLE 			0
#define DEFAULT_MAX_ANGLE 			180
#define DEFAULT_CORRECTION_ANGLE 	0

#include "main.h"
#include "stm32f7xx_hal.h"

class Servo {
private:

	uint8_t min_angle;
	uint8_t max_angle;
	int16_t correction_angle;
public:
	volatile uint32_t* PWM_Register;
	void setPulse(uint16_t pulse);
	void setAngle(int16_t angle);
	void setMinAngle(uint8_t min_angle);
	void setMaxAngle(uint8_t max_angle);
	void setCorrectionAngle(int16_t correction_angle);
	Servo(volatile uint32_t* PWM_Register);
	virtual ~Servo();
};

#endif /* SERVO_H_ */
