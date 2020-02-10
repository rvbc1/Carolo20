/*
 * Allshit.h
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */

#ifndef TASKS_ALLTASKS_H_
#define TASKS_ALLTASKS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "cmsis_os.h"

void Allshit_begin(void);

extern osThreadId GyroTaskHandle;
extern osThreadId AHRSTaskHandle;
extern osThreadId BatteryManagerHandle;
extern osThreadId ModeManagerTaskHandle;
extern osThreadId BTTaskHandle;
extern osThreadId FutabaTaskHandle;
extern osThreadId TelemetryTaskHandle;
extern osThreadId USBLinkTaskHandle;
extern osThreadId MotorControllerHandle;
extern osThreadId BuzzerTaskHandle;
extern osThreadId OdometryTaskHandle;
extern osThreadId OLEDTaskHandle;



#ifdef __cplusplus
 }
#endif

#endif /* TASKS_ALLTASKS_H_ */
