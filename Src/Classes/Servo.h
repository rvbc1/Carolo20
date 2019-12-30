/*
 * Servo.h
 *
 *  Created on: 17.09.2018
 *      Author: mice
 */

#ifndef CLASSES_SERVO_H_
#define CLASSES_SERVO_H_

#include "stdint.h"

#define FRONT_SERVO_TIM  htim2
#define BACK_SERVO_TIM	htim2
#define FRONT_SERVO_CHANNEL TIM_CHANNEL_2
#define BACK_SERVO_CHANNEL  TIM_CHANNEL_4
#define FRONT_SERVO_CCR CCR2
#define BACK_SERVO_CCR  CCR4

enum{FRONT_SERVO, BACK_SERVO }typedef servo_num;

class Servo {
	uint16_t pwm_middle;
	uint16_t pwm_band;
	uint16_t pwm_last;

	float max_degrees;
	float max_radians;


	servo_num current_servo;

	uint8_t tim_running = 0;
	void SetPWM(uint16_t value);
	uint16_t GetPWM(void);


	float current_angle = 0.f;
	float set_angle = 0.f;
	float set_velocity = 0.f;
public:
	void Init(void);
	void PositionTracking(void);

	void Disarm(void);
	void Arm(void);
	void SetAngleD(float angle = 0.f, float velocity = 0.f);
	void SetAngleR(float angle = 0.f, float velocity = 0.f);

	Servo(uint16_t middle = 1500, uint16_t band = 500, float angle = 45.f, servo_num serv = FRONT_SERVO);
	virtual ~Servo();
};
extern Servo servo_front;
extern Servo servo_back;
#endif /* CLASSES_SERVO_H_ */
