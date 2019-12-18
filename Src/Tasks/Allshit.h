/*
 * Allshit.h
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */

#ifndef TASKS_ALLSHIT_H_
#define TASKS_ALLSHIT_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "cmsis_os.h"

void Allshit_begin(void);

extern osThreadId GyroTaskHandle;
extern osThreadId AHRSTaskHandle;
extern osThreadId BatteryManagerHandle;
extern osThreadId SteeringTaskHandle;
extern osThreadId BTTaskHandle;
extern osThreadId FutabaTaskHandle;
extern osThreadId TelemetryTaskHandle;
extern osThreadId USBTaskHandle;
extern osThreadId MotorControllerHandle;
extern osThreadId BuzzerTaskHandle;
extern osThreadId OdometryTaskHandle;
extern osThreadId OLEDTaskHandle;

extern uint16_t cnt_blueled;
extern uint8_t vision_reset;
extern uint8_t vision_reset_sent;
extern uint8_t vision_reset_ack;



#ifdef __cplusplus
 }
#endif

#endif /* TASKS_ALLSHIT_H_ */
