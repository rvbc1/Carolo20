/*
 * Motor.cpp
 *
 *  Created on: 22.06.2018
 *      Author: mice
 */

#include <AllTasks.h>
#include <LightsManager.h>
#include <Motor.h>
#include "PowerManager.h"
#include "Tools.h"
#include "usart.h"
#include "tim.h"

#define ELM 100


Motor motor(1500, 500);

float avrg_current_speed;
uint32_t avrg_counter;

#ifdef VESC

static void send_packet(unsigned char *data, unsigned int len) {
	HAL_UART_Transmit_IT(&huart7, data,len);
}

void UART_Communication_Init(void) {
	bldc_interface_uart_init(send_packet);
}

uint8_t UART_data_buffer [10];


void set_duty_cycle(float dutyCycle){  //Sending duty cycle to VESC without library
	UART_data_buffer[0] = 2;
	UART_data_buffer[1] = 5;
	UART_data_buffer[2] = 5;

	int32_t buf = (int32_t)(dutyCycle * 100000.0);
	UART_data_buffer[3] = buf >> 24;
	UART_data_buffer[4] = buf >> 16;
	UART_data_buffer[5] = buf >> 8;
	UART_data_buffer[6] = buf;
	uint16_t crc = crc16(&UART_data_buffer[2] , 5);
	UART_data_buffer[7] = crc >> 8;
	UART_data_buffer[8] = crc;
	HAL_UART_Transmit_IT(&huart7, UART_data_buffer, 10);
}
#endif


void Motor::Init(){
	MX_TIM3_Init();
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

#ifdef PWM_ESC
	MX_TIM4_Init();
	SetPWM(pwm_middle);
#endif

#ifdef VESC
	MX_UART7_Init();
	UART_Communication_Init();
#endif
	SetPassthroughState(true);
	SetDuty(0.f);
	Arm();
	//osDelay(200);
	Disarm();
	SetPassthroughState(true);
}
void Motor::Process(void) {
	Read();
	Conversions();
	SpeedTracking();
	Controller();
#ifdef PWM_ESC
	if (tim_running)
#endif
		Output();

	osDelay(3);
	//osDelay(_dt * 1000.f);
}
void Motor::Read(void){
	static int16_t oldCount = 0;
	int16_t count = TIM3->CNT;
	impulses = - (count - oldCount);
	oldCount = count;
	totalImpulses += impulses;
}
void Motor::Conversions(void) {
	float filtered_impulses = lpf.apply(impulses);

	current_rpm = filtered_impulses * enc_to_rpm;
	rotations = totalImpulses * enc_to_rotations;

	current_velocity = filtered_impulses * enc_to_mms;

	distance = totalImpulses * enc_to_mm;


}
void Motor::Controller(void){

	static float previous_error = 0;

	int32_t now = tools.GetMicros();
	static int32_t before = now;
	float dt = constrainf((now - before) * 1e-6F, (_dt/2), (_dt*2));

	before = now;

	set_rpm = current_set_velocity / rpm_to_mms;

	float setpoint = current_set_velocity;
	float measurement = current_velocity;


	float error = setpoint - measurement;

	if(measurement > 1000){
		Proportional = Kp * error * 1.2;
		//	} else if(setpoint >1000){
		//		Proportional = Kp * error * 1.3;
	} else {
		Proportional = Kp * error;
	}


	Integral += Ki *error * dt;
	Integral = constrainf(Integral, -windup_limit, windup_limit);

	Derivative = Kd * dterm_lpf.apply(error - previous_error) / dt;
	previous_error = error;

	if (controller_en) {
		float vBatScaling = 1.f;
		if (powermanager.voltage > 6.f)
			vBatScaling = 8.4f / powermanager.voltage;
		pid_value = vBatScaling * (Proportional + Integral + Derivative);
		pid_value += SIGNF(pid_value) * duty_deadband;
		pid_value = constrainf(pid_value, -1.f, 1.f);
	} else {
		previous_error = 0.f;
		Integral = 0.f;
		pid_value = 0.f;
	}

	if (setpoint < 150 && setpoint > -150) {
		pid_value = 0.f;
		Integral = 0.f;
	}
}
void Motor::Arm(void) {
	controller_en = true;
#ifdef PWM_ESC
	tim_running = true;
#endif
}
void Motor::Disarm(void) {
	controller_en = false;
#ifdef PWM_ESC
	tim_running = false;
#endif
#ifdef VESC
	bldc_interface_set_duty_cycle(0.f);
#endif
}
float Motor::getRPMs(void){
	return current_rpm;
}
float Motor::getVelocity(void){
	return current_velocity;
}
float Motor::getSetVelocity(void){
	return current_set_velocity;
}
float Motor::getDistance(void){
	return distance;
}
int32_t Motor::getImpulses(void){
	return totalImpulses;
}
float Motor::getMaxVelocity(void){
	return max_velocity;
}
float Motor::getAcceleration(void){
	return current_acceleration;
}
float Motor::getPIDvalue(void){
	return pid_value;
}

void Motor::setMaxVelocity(float velocity){
	max_velocity = velocity;
}
void Motor::SpeedTracking(void) {
	Arm();
	int32_t now = tools.GetMicros();
	static int32_t before = now;
	if (set_acceleration) {
		float dt = (now - before) * 1e-6F;

		//		if (set_jerk) {
		//			current_acceleration += SIGNF(set_acceleration - current_acceleration) * set_jerk * dt;
		//			constrainf(current_acceleration, -max_acceleration, max_acceleration);
		//		} else {
		//			current_acceleration = set_acceleration;
		//		}
		current_acceleration = (current_velocity - previous_velocity) / dt;

		current_set_velocity += SIGNF(set_velocity - current_set_velocity) * set_acceleration * dt;
		constrainf(current_set_velocity, -max_velocity, max_velocity);

	} else {
		current_set_velocity = set_velocity;
	}
	before = now;
}
void Motor::SetVelocity(float velocity, float acceleration, float jerk) {
	set_velocity = constrainf(velocity, -max_velocity, max_velocity);
	set_acceleration = constrainf(acceleration, -max_acceleration, max_acceleration);
	set_jerk = jerk;
}
void Motor::SetDuty(float duty) {
	set_duty = constrainf(duty, -1.f, 1.f);
}

void Motor::SetControllerState(bool is_enabled){
	controller_en = is_enabled;
}
void Motor::SetPassthroughState(bool is_passthrough){
	passthrough = is_passthrough;
}
#ifdef VESC
void Motor::Output(void) {
	if (passthrough) {
		bldc_interface_set_duty_cycle(set_duty);
	} else{
		bldc_interface_set_duty_cycle(pid_value);
	}
}
Motor::Motor(uint16_t middle, uint16_t band){
}
#endif

#ifdef PWM_ESC
void Motor::Output(void) {
	if (passthrough) {
		SetPWM(set_duty * pwm_band + pwm_middle);
	} else
		SetPWM(pid_value * pwm_band + pwm_middle);
}
void Motor::SetPWM(uint16_t value){
	TIM4->CCR4 = value;
}
uint16_t Motor::GetPWM(void){
	return TIM4->CCR4;
}
Motor::Motor(uint16_t middle, uint16_t band): pwm_middle(middle), pwm_band(band) {
	pwm_last = pwm_middle;
}
#endif
Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

