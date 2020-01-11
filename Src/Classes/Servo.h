/*
 * Servo.h
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#ifndef SERVO_H_
#define SERVO_H_

#define MIN_PWM_WIDTH 				1000
#define MAX_PWM_WIDTH 				1900
#define DEFAULT_MIN_ANGLE 			0
#define DEFAULT_MAX_ANGLE 			180
#define DEFAULT_MIN_USER_ANGLE 		DEFAULT_MIN_ANGLE
#define DEFAULT_MAX_USER_ANGLE 		DEFAULT_MAX_ANGLE
#define DEFAULT_CORRECTION_ANGLE 	0

#include "main.h"
#include "tim.h"

class Servo {
private:
	uint8_t timer_is_on = false;
	volatile uint32_t* PWM_Register;
	int16_t min_angle;
	int16_t max_angle;
	int16_t min_user_angle;
	int16_t max_user_angle;
	int16_t correction_angle;
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	volatile uint32_t* getPWM_Register();
public:
	void Arm();
	void Disarm();
	void setPWM_REG(volatile uint32_t* PWM_Register);
	void setPWM(uint16_t pwm);
	void setAngle(int16_t angle);
	void setMinAngle(int16_t min_angle);
	void setMaxAngle(int16_t max_angle);
	void setCorrectionAngle(int16_t correction_angle);
	Servo(TIM_HandleTypeDef* htim, uint32_t channel);
	virtual ~Servo();
};

#endif /* SERVO_H_ */
