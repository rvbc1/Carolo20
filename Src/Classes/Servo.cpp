/*
 * Servo.cpp
 *
 *  Created on: Apr 7, 2019
 *      Author: rvbc-
 */

#include "Servo.h"

Servo::Servo(volatile uint32_t* PWM_Register) {
	this->PWM_Register = PWM_Register;
	this->max_angle = DEFAULT_MAX_ANGLE;
	this->min_angle = DEFAULT_MIN_ANGLE;
	this->correction_angle = DEFAULT_CORRECTION_ANGLE;
}

void Servo::setPulse(uint16_t pulse){
	if(pulse < MIN_PULSE_WIDTH) pulse = MIN_PULSE_WIDTH;
	if(pulse > MAX_PULSE_WIDTH) pulse = MAX_PULSE_WIDTH;
	*PWM_Register = pulse;
}

void Servo::setAngle(int16_t angle){
	angle += correction_angle;
	if(angle < min_angle) angle = min_angle;
	if(angle > max_angle) angle = max_angle;
	double pulse = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * (angle / 180.0) + MIN_PULSE_WIDTH;
	setPulse(pulse);
}

void Servo::setMinAngle(uint8_t min_angle){
	this->min_angle = min_angle;
}

void Servo::setMaxAngle(uint8_t max_angle){
	this->max_angle = max_angle;
}

void Servo::setCorrectionAngle(int16_t correction_angle){
	this->correction_angle = correction_angle;
}

Servo::~Servo() {
	// TODO Auto-generated destructor stub
}
