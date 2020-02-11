/*
 * Encoder.cpp
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#include <Encoder.h>
#include <AllTasks.h>
#include "tim.h"
#include "Tools.h"


Encoder encoder;

void Encoder::Init(){
	MX_TIM3_Init();
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	avrgVelocity = 0.f;
}

void Encoder::Process(){
	Read();
	Conversions();

	osDelay(2);
}

void Encoder::Read(){
	static int16_t oldCount = 0;
	int16_t count = TIM3->CNT;
	impulses = - (count - oldCount);
	oldCount = count;
	totalImpulses += impulses;
}
void Encoder::Conversions(void) {
	float filtered_impulses = lpf.apply(impulses);

	current_rpm = filtered_impulses * enc_to_rpm;
	rotations = totalImpulses * enc_to_rotations;

	current_velocity = filtered_impulses * enc_to_mms;

	distance = totalImpulses * enc_to_mm;
	AccelerationCalculate();
	avrgAccelerationProcess();
	avrgVelocityProcess();

	previous_velocity = current_velocity;
}
void Encoder::AccelerationCalculate(){
	int32_t now = tools.GetMicros();
	static int32_t before = now + 1; //to avoid dividing by 0
	float dt = (now - before) * 1e-6F;
	current_acceleration = (current_velocity - previous_velocity) / dt;
	before = now;
}

void Encoder::avrgVelocityProcess(){
	float sum = 0;

	last_velocities[avrgVelocity_counter] = current_velocity;
	if(++avrgVelocity_counter >= VELOCITY_AVERAGE_NUM) avrgVelocity_counter = 0;

	for(uint32_t i = 0; i < VELOCITY_AVERAGE_NUM; i++) sum += last_velocities[i];
	avrgVelocity = sum / VELOCITY_AVERAGE_NUM;
}

void Encoder::avrgAccelerationProcess(){
	float sum = 0;

	last_accelerations[avrgAcceleration_counter] = current_acceleration;
	if(++avrgAcceleration_counter >= ACCELERATION_AVERAGE_NUM) avrgAcceleration_counter = 0;

	for(uint32_t i = 0; i < ACCELERATION_AVERAGE_NUM; i++) sum += last_accelerations[i];
	avrgAcceleration = sum / ACCELERATION_AVERAGE_NUM;
}

float Encoder::getRPMs(void){
	return current_rpm;
}
float Encoder::getVelocity(void){
	return current_velocity;
}
float Encoder::getDistance(void){
	return distance;
}
int32_t Encoder::getImpulses(void){
	return totalImpulses;
}

float Encoder::getAcceleration(void){
	return current_acceleration;
}

float Encoder::getAverageAcceleration(){
	return avrgAcceleration;
}
float Encoder::getAverageVelocity(){
	return avrgVelocity;
}
float Encoder::getRPM_To_mms_Rate() const{
	return rpm_to_mms;
}

Encoder::Encoder() {
	// TODO Auto-generated constructor stub

}

Encoder::~Encoder() {
	// TODO Auto-generated destructor stub
}

