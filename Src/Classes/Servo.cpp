/*
 * Servo.cpp
 *
 *  Created on: 17.09.2018
 *      Author: mice
 */

#include <Servo.h>
#include "Tools.h"
#include "Mathematics.h"

#include "tim.h"

Servo servo_front(1450, 450, 45.f, FRONT_SERVO);
Servo servo_back(1450, 450, 45.f, BACK_SERVO);

void Servo::Init(void){
	MX_TIM2_Init();
	if(current_servo == FRONT_SERVO)    HAL_TIM_PWM_Start(&FRONT_SERVO_TIM , FRONT_SERVO_CHANNEL);
	else if(current_servo == BACK_SERVO)HAL_TIM_PWM_Start(&BACK_SERVO_TIM,   BACK_SERVO_CHANNEL);
	SetPWM(pwm_middle);

}

void Servo::Disarm(void) {
	pwm_last = GetPWM();
	SetPWM(0);

	if (HAL_GPIO_ReadPin(O1_GPIO_Port, O1_Pin) == GPIO_PIN_RESET)
		if ((HAL_TIM_PWM_Stop(&BACK_SERVO_TIM, BACK_SERVO_CHANNEL) == HAL_OK) && (HAL_TIM_PWM_Stop(&FRONT_SERVO_TIM , FRONT_SERVO_CHANNEL) /* NEW SERVO */ == HAL_OK))
			tim_running = 0;
}
void Servo::Arm(void) {
	if (!tim_running) {
		SetPWM(pwm_last);

		if ((HAL_TIM_PWM_Start(&BACK_SERVO_TIM, BACK_SERVO_CHANNEL) == HAL_OK) && (HAL_TIM_PWM_Start(&FRONT_SERVO_TIM , FRONT_SERVO_CHANNEL) /* NEW SERVO */ == HAL_OK))
			tim_running = 1;
	}
}
void Servo::SetAngleD(float angle, float velocity) {
	angle = constrainf(angle, -45*0.95, max_degrees);
	set_angle = -angle;
	set_velocity = velocity;
}
void Servo::SetAngleR(float angle, float velocity) {
	angle = constrainf(angle, -max_radians, max_radians);
	set_angle = RADIANS_TO_DEGREES(angle);
	set_velocity = RADIANS_TO_DEGREES(velocity);
}
void Servo::PositionTracking(void) {
	Arm();
//	int32_t now = tools.GetMicros();
//	static int32_t before = now;
//	if (set_velocity == 0.f) {
//		current_angle = set_angle;
//	} else {
//		float dt = (now - before) * 1e-6F;
//		current_angle += SIGNF(set_angle - current_angle) * set_velocity * dt;
//		constrainf(current_angle, -max_degrees, max_degrees);
//	}

	//current_angle += constrainf(set_angle - current_angle, -1, 1);

	current_angle = set_angle;

	SetPWM( current_angle / max_degrees * pwm_band + pwm_middle);
	//before = now;
}

void Servo::SetPWM(uint16_t value){
	if      (current_servo == FRONT_SERVO)   FRONT_SERVO_TIM.Instance->FRONT_SERVO_CCR = value;
	else if (current_servo == BACK_SERVO)    BACK_SERVO_TIM.Instance ->BACK_SERVO_CCR  = value;
}
uint16_t Servo::GetPWM(void){
	if      (current_servo == FRONT_SERVO) return  FRONT_SERVO_TIM.Instance->FRONT_SERVO_CCR;
    else if (current_servo == BACK_SERVO)  return  BACK_SERVO_TIM.Instance->BACK_SERVO_CCR;

}
Servo::Servo(uint16_t middle, uint16_t band, float angle, servo_num serv): pwm_middle(middle), pwm_band(band), max_degrees(angle), current_servo(serv) {
	pwm_last = pwm_middle;
	max_radians = DEGREES_TO_RADIANS(max_degrees);
}

Servo::~Servo() {
	// TODO Auto-generated destructor stub
}

