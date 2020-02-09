/*
 * Servo.h
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#ifndef SERVO_H_
#define SERVO_H_

#define DEFAULT_MIN_PWM_WIDTH 		600
#define DEFAULT_MAX_PWM_WIDTH 		2500
#define DEFAULT_MIN_ANGLE 			-45
#define DEFAULT_MAX_ANGLE 			45
#define DEFAULT_MIN_USER_ANGLE 		DEFAULT_MIN_ANGLE
#define DEFAULT_MAX_USER_ANGLE 		DEFAULT_MAX_ANGLE
#define DEFAULT_CORRECTION_ANGLE 	0

#include "main.h"
#include "tim.h"

class Servo {
private:
	uint8_t timer_is_on = false;
	volatile uint32_t* PWM_Register;
	uint16_t min_pwm_width;
	uint16_t max_pwm_width;
	int16_t min_angle;
	int16_t max_angle;
	int16_t min_user_angle;
	int16_t max_user_angle;
	int16_t correction_angle;
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	volatile uint32_t* getPWM_Register();
	void setPWM_REG(volatile uint32_t* PWM_Register);
public:
	void Arm();
	void Disarm();
	void setPWM(uint16_t pwm);
	void setPWMwidth(uint16_t min, uint16_t max);
	void setAngle(int16_t angle);
	void setMinAngle(int16_t min_angle);
	void setMaxAngle(int16_t max_angle);
	void setMinUserAngle(int16_t min_user_angle);
	void setCorrectionAngle(int16_t correction_angle);
	Servo(TIM_HandleTypeDef* htim, uint32_t channel);
	virtual ~Servo();
};

#endif /* SERVO_H_ */
