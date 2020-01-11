/*
 * Servo.cpp
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#include "Servo.h"


Servo::Servo(TIM_HandleTypeDef* htim, uint32_t channel) {
	this->htim = htim;
	this->channel = channel;
	this->PWM_Register = getPWM_Register();
	this->max_angle = DEFAULT_MAX_ANGLE;
	this->min_angle = DEFAULT_MIN_ANGLE;
	this->max_user_angle = DEFAULT_MAX_USER_ANGLE;
	this->min_user_angle = DEFAULT_MIN_USER_ANGLE;
	this->correction_angle = DEFAULT_CORRECTION_ANGLE;
	timer_is_on = false;
}

volatile uint32_t* Servo::getPWM_Register(){
	if(channel == TIM_CHANNEL_1){
		return &htim->Instance->CCR1;
	} else if(channel == TIM_CHANNEL_2){
		return &htim->Instance->CCR2;
	} else if(channel == TIM_CHANNEL_3){
		return &htim->Instance->CCR3;
	} else if(channel == TIM_CHANNEL_4){
		return &htim->Instance->CCR4;
	} else if(channel == TIM_CHANNEL_5){
		return &htim->Instance->CCR5;
	} else {
		return &htim->Instance->CCR6;
	}
}

void Servo::Arm(){
	if(!timer_is_on){
		HAL_TIM_PWM_Start(htim, channel);
		timer_is_on = true;
	}
}

void Servo::Disarm(){
	if(timer_is_on){
		HAL_TIM_PWM_Stop(htim, channel);
		timer_is_on = false;
	}
}

void Servo::setPWM(uint16_t pwm){
	if(pwm < MIN_PWM_WIDTH) pwm = MIN_PWM_WIDTH;
	if(pwm > MAX_PWM_WIDTH) pwm = MAX_PWM_WIDTH;
	*PWM_Register = pwm;
}

void Servo::setAngle(int16_t angle){
	angle += correction_angle;
	if(angle < min_angle) angle = min_angle;
	if(angle > max_angle) angle = max_angle;
	double pwm = (MAX_PWM_WIDTH - MIN_PWM_WIDTH) * (angle / (DEFAULT_MAX_ANGLE - DEFAULT_MIN_ANGLE)) + MIN_PWM_WIDTH;
	setPWM(pwm);
}

void Servo::setMinAngle(int16_t min_angle){
	this->min_angle = min_angle;
}

void Servo::setMaxAngle(int16_t max_angle){
	this->max_angle = max_angle;
}

void Servo::setCorrectionAngle(int16_t correction_angle){
	this->correction_angle = correction_angle;
}

Servo::~Servo() {
	// TODO Auto-generated destructor stub
}
