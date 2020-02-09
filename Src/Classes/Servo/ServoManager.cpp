/*
 * ServoManager.cpp
 *
 *  Created on: Jan 11, 2020
 *      Author: rvbc-
 */

#include <ServoManager.h>
#include "ModeManager.h"
#include "USBLink.h"
#include "Futaba.h"

ServoManager servo_manager;

void ServoManager::init(){
	MX_TIM2_Init();

	servo_back = new Servo(&htim2, TIM_CHANNEL_2);
	servo_back->setCorrectionAngle(-10);
	servo_front = new Servo(&htim2, TIM_CHANNEL_4);
	//servo_front->setCorrectionAngle(-36);
}

void ServoManager::arm(){
	servo_front->Arm();
	servo_back->Arm();
}

void ServoManager::disarm(){
	servo_front->Disarm();
	servo_back->Disarm();
}

void ServoManager::setAngle(int16_t front, int16_t back){
	servo_front->setAngle(-front);	//check why signal is inverted
	servo_back->setAngle(-back);	//check why signal is inverted
}

void ServoManager::process(){

	switch(steering_manager.getDriveMode()){
	case ModeManager::DISABLE:
		disarm();
		break;
	case ModeManager::ENABLE:
		arm();
		break;
	}

	switch(steering_manager.getRCmode()){
	case ModeManager::DISARMED:
		//disarm();
		break;
	case ModeManager::MODE_ACRO:
		setAngle(int16_t(futaba.SmoothDeflection[YAW] * 45.f), -int16_t(futaba.SmoothDeflection[YAW] * 45.f));
		break;
	case ModeManager::MODE_SEMI:
	case ModeManager::MODE_AUTONOMOUS:
		setAngle(setpoints_from_vision.fi_front, setpoints_from_vision.fi_back);
		break;
	}
	osDelay(1);
}

ServoManager::ServoManager() {
	// TODO Auto-generated constructor stub

}

ServoManager::~ServoManager() {
	// TODO Auto-generated destructor stub
}

