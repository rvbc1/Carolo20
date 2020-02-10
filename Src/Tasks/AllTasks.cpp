/*
 * Allshit.cpp
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */



#include <AllTasks.h>
#include <ArcoNotifierLED.h>
#include <ButtonsManager.h>
#include <LightsManager.h>
#include <ModeManager.h>
#include "cmsis_os.h"
#include "Futaba.h"
#include "Gyro.h"
#include "PowerManager.h"
#include "AHRS.h"
#include "Motor.h"
#include "Tools.h"
#include "Telemetry.h"
#include "Buzzer.h"
#include "ServoManager.h"
#include "MotorManager.h"
#include "Bluetooth.h"
#include "Odometry.h"
#include "crc.h"
#include "OLED.h"
#include "ButtonsManager.h"
#include "Mathematics.h"
#include "tim.h"
#include "WatchDogs.h"
#include "USBLink.h"

osThreadId GyroTaskHandle;
osThreadId AHRSTaskHandle;
osThreadId BatteryManagerHandle;
osThreadId ModeManagerTaskHandle;
osThreadId BTTaskHandle;
osThreadId FutabaTaskHandle;
osThreadId TelemetryTaskHandle;
osThreadId USBLinkTaskHandle;
osThreadId MotorControllerHandle;
osThreadId BuzzerTaskHandle;
osThreadId OdometryTaskHandle;
osThreadId OLEDTaskHandle;
osThreadId LightsTaskHandle;
osThreadId ButtonsTaskHandle;
osThreadId WatchDogsTaskHandle;
osThreadId AcroNotifierLEDTaskHandle;

osThreadId ServoManagerTaskHandle;
osThreadId MotorManagerTaskHandle;


void StartGyroTask(void const * argument);
void StartAHRSTask(void const * argument);
void StartOdometryTask(void const * argument);
void StartBatteryManager(void const * argument);
void StartModeManagerTask(void const * argument);
void StartBTTask(void const * argument);
void StartFutabaTask(void const * argument);
void StartUSBLinkTask(void const * argument);
void StartTelemetryTask(void const * argument);
void StartMotorController(void const * argument);
void StartBuzzerTask(void const * argument);
void StartOLEDTask(void const * argument);
void StartLightsTask(void const * argument);
void StartButtonsTask(void const * argument);
void StartWatchDogsTask(void const * argument);

void StartAcroNotifierLEDTask(void const * argument);


void StartServoManagerTask(void const * argument);
void StartMotorManagerTask(void const * argument);


void Allshit_begin(void) {

	/* definition and creation of FutabaTask */
	osThreadDef(FutabaTask, StartFutabaTask, osPriorityHigh, 0, 256);
	FutabaTaskHandle = osThreadCreate(osThread(FutabaTask), NULL);

	/* definition and creation of MotorController */
	osThreadDef(MotorController, StartMotorController, osPriorityHigh, 0, 512);
	MotorControllerHandle = osThreadCreate(osThread(MotorController), NULL);

	/* definition and creation of SteeringTask */
	osThreadDef(ModeManagerTask, StartModeManagerTask, osPriorityHigh, 0, 512);
	ModeManagerTaskHandle = osThreadCreate(osThread(ModeManagerTask), NULL);

	/* definition and creation of GyroTask */
	osThreadDef(GyroTask, StartGyroTask, osPriorityHigh, 0, 1024);
	GyroTaskHandle = osThreadCreate(osThread(GyroTask), NULL);

	/* definition and creation of AHRSTask */
	osThreadDef(AHRSTask, StartAHRSTask, osPriorityHigh, 0, 256);
	AHRSTaskHandle = osThreadCreate(osThread(AHRSTask), NULL);

	/* Odometry - HIGH PRIORITY*/
	osThreadDef(OdometryTask, StartOdometryTask, osPriorityHigh, 0, 128);
	OdometryTaskHandle = osThreadCreate(osThread(OdometryTask), NULL);

	/* definition and creation of BatteryManager */
	osThreadDef(BatteryManager, StartBatteryManager, osPriorityBelowNormal, 0, 256);
	BatteryManagerHandle = osThreadCreate(osThread(BatteryManager), NULL);

	/* definition and creation of USBLink */
	osThreadDef(USBLink, StartUSBLinkTask, osPriorityHigh, 0, 256);
	USBLinkTaskHandle = osThreadCreate(osThread(USBLink), NULL);

	/* definition and creation of TelemetryTask */
	osThreadDef(TelemetryTask, StartTelemetryTask, osPriorityNormal, 0, 256);
	TelemetryTaskHandle = osThreadCreate(osThread(TelemetryTask), NULL);

	/* definition and creation of BTTask */
	osThreadDef(BTTask, StartBTTask, osPriorityLow, 0, 256);
	BTTaskHandle = osThreadCreate(osThread(BTTask), NULL);

	/* Buzzer - LOW PRIORITY */
	osThreadDef(BuzzerTask, StartBuzzerTask, osPriorityLow, 0, 128);
	BuzzerTaskHandle = osThreadCreate(osThread(BuzzerTask), NULL);

	/* OLED - LOW PRIORITY */
//	osThreadDef(OLEDTask, StartOLEDTask, osPriorityLow, 0, 256);
//	OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

	/* Lights - ws2812 - MEDIUM PRIORITY */
	osThreadDef(LightsTask, StartLightsTask, osPriorityNormal, 0, 1024);
	LightsTaskHandle = osThreadCreate(osThread(LightsTask), NULL);

	/* Buttons - LOW PRIORITY */
	osThreadDef(ButtonsTask, StartButtonsTask, osPriorityLow, 0, 128);
	ButtonsTaskHandle = osThreadCreate(osThread(ButtonsTask), NULL);

	/* Servo - MEDIUM PRIORITY */
	osThreadDef(ServoManagerTask, StartServoManagerTask, osPriorityNormal, 0, 128);
	ServoManagerTaskHandle = osThreadCreate(osThread(ServoManagerTask), NULL);

	/* MotorManager - LOW PRIORITY */
	osThreadDef(MotorManagerTask, StartMotorManagerTask, osPriorityLow, 0, 128);
	MotorManagerTaskHandle = osThreadCreate(osThread(MotorManagerTask), NULL);

	/* definition and creation of WatchDogsTask */
	osThreadDef(WatchDogsTask, StartWatchDogsTask, osPriorityHigh, 0, 512);
	WatchDogsTaskHandle = osThreadCreate(osThread(WatchDogsTask), NULL);

	/* LEDup - LOW PRIORITY */
	osThreadDef(AcroNotifierLEDTask, StartAcroNotifierLEDTask, osPriorityLow, 0, 256);
	AcroNotifierLEDTaskHandle = osThreadCreate(osThread(AcroNotifierLEDTask), NULL);

}

void StartFutabaTask(void const * argument) {
	futaba.Init();
	while(true){
		futaba.Process();
	}

}

void StartMotorController(void const * argument) {
	motor.Init();
	while(true) {
		motor.Process();
		osSignalSet(OdometryTaskHandle, odometry.SignalReady);
	}
}

void StartModeManagerTask(void const * argument) {
	mode_manager.init();
	while(true){
		mode_manager.proccess();
	}
}

void StartGyroTask(void const * argument) {
	gyro.Init();
	while(true) {
		gyro.Process();
	}
}

void StartAHRSTask(void const * argument) {
	ahrs.Init();
	while(true) {
		ahrs.Process();
		osSignalSet(USBLinkTaskHandle, USB_TX_signal);
	}
}


void StartOdometryTask(void const * argument) {
	odometry.Init();
	while(true) {
		osSignalWait(odometry.SignalReady, osWaitForever);
		odometry.Process(ahrs.attitude.values.yaw, motor.getDistance(), tools.GetMicros());
	}
}

void StartBatteryManager(void const * argument) {
	powermanager.Init();
	while(true){
		powermanager.Handler();
	}

}

void StartUSBLinkTask(void const * argument) {
	USBLink::initHardware();
	while(true){
		usb_link.USB_Process();
	}
}

void StartTelemetryTask(void const * argument) {
	telemetry.Init();
	while(true){
		telemetry.Process();
	}

}

void StartBTTask(void const * argument) {
	Bluetooth_Init();
	while(true){
		Bluetooth_Process();
	}
}

void StartBuzzerTask(void const * argument){
	buzzer.Init();
	while(true){
		buzzer.Loop();
	}
}

void StartOLEDTask(void const * argument){
	oled.Init();
	while(true){
		oled.process();
	}
}


void StartLightsTask(void const * argument){
	lights_manager.ws2812_init();
	while(true){
		lights_manager.process();
	}
}


void StartButtonsTask(void const * argument){
	buttons_manager.Init();
	while(true){
		buttons_manager.process();
	}
}

void StartWatchDogsTask(void const * argument){
//	while(HAL_GetTick() < 500){
//		osDelay(1);
//	}
//	WatchDogs::init();
	while(true){
		WatchDogs::process();
	}
}


void StartAcroNotifierLEDTask(void const * argument){
	acro_notifier_led.Init();
	while(true){
		acro_notifier_led.Process();
	}
}

void StartServoManagerTask(void const * argument){
	servo_manager.init();
	while(true){
		servo_manager.process();
	}
}

void StartMotorManagerTask(void const * argument){
	motor_manager.init();
	while(true){
		motor_manager.process();

	}
}
