/*
 * Encoder.h
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#ifndef CLASSES_MOTOR_ENCODER_H_
#define CLASSES_MOTOR_ENCODER_H_

#include "stdint.h"
#include "Filters.h"
#include "Mathematics.h"
#include "Average.h"

#define ACCELERATION_AVERAGE_NUM 100
#define VELOCITY_AVERAGE_NUM 50



class Encoder {
public:
	void Init();
	void Read();
	void Conversions();
	void Process();

	float getRPMs();
	float getVelocity();
	float getDistance();
	int32_t getImpulses();
	float getAcceleration();

	float getAverageAcceleration();
	float getAverageVelocity();
	float getRPM_To_mms_Rate() const;


	Encoder();
	virtual ~Encoder();
private:
	/* Encoders Parameters */
	const float impulses_per_revolution = 1024*10.f;

	const float wheel_diameter = 65.f;
	const float gear_invratio = 10.f/6.f;
	const float _dt = 0.002f;

	const float enc_to_mm = M_PI_FLOAT * wheel_diameter * gear_invratio / impulses_per_revolution;
	const float enc_to_mms = M_PI_FLOAT * wheel_diameter * gear_invratio / impulses_per_revolution / _dt;

	const float enc_to_rotations = 1.f/ (impulses_per_revolution);
	const float enc_to_rpm = 60.f / impulses_per_revolution / _dt;

	const float rpm_to_mms = M_PI_FLOAT * wheel_diameter * gear_invratio / 60.f ;

	/* Measurements & Setpoints*/
	int32_t totalImpulses = 0;
	int16_t impulses = 0;

	float distance = 0.f;
	float rotations = 0.f;

	float avrgVelocity;
	float avrgAcceleration;

	float current_rpm = 0.f;
	float current_velocity = 0.f;
	float previous_velocity = 0.f;
	float current_acceleration = 0.f;

	PT1Filter lpf = PT1Filter(50, _dt);

	/* Average variables */
	float last_accelerations[ACCELERATION_AVERAGE_NUM]= {0};
	float last_velocities[VELOCITY_AVERAGE_NUM] = {0};
	uint32_t avrgVelocity_counter = 0;
	uint32_t avrgAcceleration_counter = 0;

//	Average avrgVel(VELOCITY_AVERAGE_NUM);


	void avrgVelocityProcess();
	void avrgAccelerationProcess();
	void AccelerationCalculate();
};

extern Encoder encoder;

#endif /* CLASSES_MOTOR_ENCODER_H_ */
