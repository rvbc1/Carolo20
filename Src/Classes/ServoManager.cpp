/*
 * ServoManager.cpp
 *
 *  Created on: Jan 11, 2020
 *      Author: Marek
 */

#include <ServoManager.h>

ServoManager servo_manager;

void ServoManager::init(){
	MX_TIM2_Init();

	servo_back = new Servo(&htim2, TIM_CHANNEL_2);
	servo_front = new Servo(&htim2, TIM_CHANNEL_4);
}

void ServoManager::arm(){
	servo_front->Arm();
	servo_back->Arm();
}

void ServoManager::disarm(){
	servo_front->Disarm();
	servo_back->Disarm();
}

void ServoManager::setAngle(uint16_t front, uint16_t back){
	servo_front->setAngle(front);
	servo_back->setAngle(back);
}

ServoManager::ServoManager() {
	// TODO Auto-generated constructor stub

}

ServoManager::~ServoManager() {
	// TODO Auto-generated destructor stub
}

