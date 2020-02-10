/*
 * Motor.h
 *
 *  Created on: 22.06.2018
 *      Author: mice
 */

#ifndef CLASSES_MOTOR_H_
#define CLASSES_MOTOR_H_

#define VESC
//#define PWM_ESC

#include "stdint.h"
#include "Filters.h"
#include "Mathematics.h"

#ifdef VESC
#include "crc.h"
#include "bldc_interface.h"
#include "bldc_interface_uart.h"
#endif

class Motor {
#ifdef PWM_ESC
	/* Timer Parameters */
	uint16_t pwm_middle;
	uint16_t pwm_band;
	uint16_t pwm_last;

	uint8_t tim_running = 0;
	void SetPWM(uint16_t value);
	uint16_t GetPWM(void);
#endif

	const float _dt = 0.002f;

	/* Motor Parameters */
	float duty_deadband = 0.f;
	float max_rpm = 60000.f;
	float max_velocity = 2500.f;
	float max_acceleration = 40000.f;

	/* Controller Parameters */
//	PT1Filter lpf = PT1Filter(50, _dt);
	PT1Filter dterm_lpf = PT1Filter(40, _dt);

	float Kp = 60.0e-5f;
	float Ki = 25.0e-5f;
	float Kd = 2.0e-8f;

	float windup_limit = 0.005f;

	float Proportional, Integral = 0.f, Derivative;
	float pid_value;

	float prev_pid_value = 0;


	/* Measurements & Setpoints*/
//	int32_t totalImpulses = 0;
//	int16_t impulses = 0;
//
//	float distance = 0.f;
//	float rotations = 0.f;

	float set_duty = 0.f;
	float prev_set_duty = 0.f;
	bool passthrough = false;

	bool controller_en = false;

//	float current_rpm = 0.f;
//	float current_velocity = 0.f;
//	float previous_velocity = 0.f;
	float current_set_velocity = 0.f;
//	float current_acceleration = 0.f;

	float set_velocity = 0.f;
	float set_acceleration = 0.f;
	float set_jerk = 0.f;

	float set_rpm = 0.f;

	void Read(void);
	void Conversions(void);
	void SpeedTracking(void);
	void Controller(void);
	void Output(void);
public:

	void Init(void);
	void Process(void);

	void Arm(void);
	void Disarm(void);

	void SetControllerState(bool is_enabled);
	void SetPassthroughState(bool is_passthrough);

	void SetDuty(float duty = 0.f);
	void SetVelocity(float velocity = 0.f, float acceleration = 0.f, float jerk = 0.f);
	void setMaxVelocity(float velocity);

	float getSetVelocity(void);
	float getMaxVelocity(void);
	float getPIDvalue(void);

	Motor(uint16_t middle, uint16_t band);
	virtual ~Motor();
};
extern Motor motor;
#endif /* CLASSES_MOTOR_H_ */
