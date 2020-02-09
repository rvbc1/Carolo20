/*
 * MotorManager.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: rvbc-
 */

#include <MotorManager.h>

MotorManager motor_manager;

void MotorManager::init(){
	motor.SetPassthroughState(false);
	//motor.setMaxVelocity(3500.f);
	motor.setMaxVelocity(6000.f);
}

void MotorManager::process(){

	setMaxVelocity();
	DriveModeCheck();
	RCModeCheck();
	osDelay(1);
}

void MotorManager::DriveModeCheck(){
	switch(steering_manager.getDriveMode()){
	case ModeManager::DISABLE:
		motor.Disarm();
		break;
	case ModeManager::ENABLE:
		motor.Arm();
		break;
	}
}
void MotorManager::RCModeCheck(){
	switch(steering_manager.getRCmode()){
		case ModeManager::DISARMED:
			//motor.Disarm();
			break;
		case ModeManager::MODE_ACRO:
			motor.SetDuty(futaba.SmoothDeflection[PITCH]);
			motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 10000.f, 50000.f);
			break;
		case ModeManager::MODE_SEMI:
			motor.SetDuty(futaba.SmoothDeflection[PITCH]);
			motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 3000.f, 50000.f);
			break;
		case ModeManager::MODE_AUTONOMOUS:
			motor.SetVelocity(setpoints_from_vision.velocity, setpoints_from_vision.acceleration, setpoints_from_vision.jerk);
			break;
		}
}
void MotorManager::setMaxVelocity(){
	if(steering_manager.getServiceMode() == ModeManager::CUP){
		switch(steering_manager.getRCmode()){
		case ModeManager::DISARMED:
			//motor.Disarm();
			break;
		case ModeManager::MODE_ACRO:
			motor.setMaxVelocity(ACRO_MAX_VELOCITY);
			break;
		case ModeManager::MODE_SEMI:
			motor.setMaxVelocity(SEMI_MAX_VELOCITY);
			break;
		case ModeManager::MODE_AUTONOMOUS:
			motor.setMaxVelocity(AUTONOMOUS_MAX_VELOCITY);
			break;
		}
	}else{
		motor.setMaxVelocity(SERVICE_MAX_VELOCITY);
	}
}

MotorManager::MotorManager() {
	// TODO Auto-generated constructor stub

}

MotorManager::~MotorManager() {
	// TODO Auto-generated destructor stub
}

