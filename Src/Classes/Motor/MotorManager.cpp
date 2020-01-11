/*
 * MotorManager.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: rvbc-
 */

#include <MotorManager.h>

void MotorManager::init(){
	motor.SetPassthroughState(false);
	//motor.setMaxVelocity(3500.f);
	motor.setMaxVelocity(6000.f);
}

void MotorManager::process(){
	motor.Arm();
	switch(steering_manager.getRCmode()){
	case SteeringManager::DISARMED:
		motor.Disarm();
		break;
	case SteeringManager::MODE_ACRO:
		motor.SetDuty(futaba.SmoothDeflection[PITCH]);
		motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 10000.f, 50000.f);
		break;
	case SteeringManager::MODE_SEMI:
		motor.SetDuty(futaba.SmoothDeflection[PITCH]);
		motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 3000.f, 50000.f);
		break;
	case SteeringManager::MODE_AUTONOMOUS:
		motor.SetVelocity(setpoints_from_vision.velocity, setpoints_from_vision.acceleration, setpoints_from_vision.jerk);
		break;
	}
}

MotorManager::MotorManager() {
	// TODO Auto-generated constructor stub

}

MotorManager::~MotorManager() {
	// TODO Auto-generated destructor stub
}

