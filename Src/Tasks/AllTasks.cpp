/*
 * Allshit.cpp
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */



#include <AllTasks.h>
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
#include "LEDUp.h"

#include "USBLink.h"

osThreadId GyroTaskHandle;
osThreadId AHRSTaskHandle;
osThreadId BatteryManagerHandle;
osThreadId ModeManagerTaskHandle;
osThreadId BTTaskHandle;
osThreadId FutabaTaskHandle;
osThreadId TelemetryTaskHandle;
osThreadId USBTaskHandle;
osThreadId MotorControllerHandle;
osThreadId BuzzerTaskHandle;
osThreadId OdometryTaskHandle;
osThreadId OLEDTaskHandle;
osThreadId LightsTaskHandle;
osThreadId ButtonsTaskHandle;
osThreadId WatchDogsTaskHandle;

osThreadId LEDUpTaskHandle;

osThreadId ServoManagerTaskHandle;
osThreadId MotorManagerTaskHandle;


void StartGyroTask(void const * argument);
void StartAHRSTask(void const * argument);
void StartOdometryTask(void const * argument);
void StartBatteryManager(void const * argument);
void StartModeManagerTask(void const * argument);
void StartBTTask(void const * argument);
void StartFutabaTask(void const * argument);
void StartUSBTask(void const * argument);
void StartTelemetryTask(void const * argument);
void StartMotorController(void const * argument);
void StartBuzzerTask(void const * argument);
void StartOLEDTask(void const * argument);
void StartLightsTask(void const * argument);
void StartButtonsTask(void const * argument);
void StartWatchDogsTask(void const * argument);

void StartLEDUpTask(void const * argument);


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

	/* definition and creation of USBTask */
	//	osThreadDef(USBTask, StartUSBTask, osPriorityHigh, 0, 256);
	//USBTaskHandle = osThreadCreate(osThread(USBTask), NULL);
	osThreadDef(USBLink, StartUSBTask, osPriorityHigh, 0, 256);
	USBTaskHandle = osThreadCreate(osThread(USBLink), NULL);

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
	osThreadDef(OLEDTask, StartOLEDTask, osPriorityLow, 0, 256);
	OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

	/* Lights - ws2812 - MEDIUM PRIORITY */
	osThreadDef(LightsTask, StartLightsTask, osPriorityNormal, 0, 1024);
	LightsTaskHandle = osThreadCreate(osThread(LightsTask), NULL);

	osThreadDef(ButtonsTask, StartButtonsTask, osPriorityLow, 0, 128);
	ButtonsTaskHandle = osThreadCreate(osThread(ButtonsTask), NULL);



	//SERVO
	osThreadDef(ServoManagerTask, StartServoManagerTask, osPriorityLow, 0, 128);
	ServoManagerTaskHandle = osThreadCreate(osThread(ServoManagerTask), NULL);

	//MOTOR
	osThreadDef(MotorManagerTask, StartMotorManagerTask, osPriorityLow, 0, 128);
	MotorManagerTaskHandle = osThreadCreate(osThread(MotorManagerTask), NULL);

	/* definition and creation of WatchDogsTask */

//	osThreadDef(WatchDogsTask, StartWatchDogsTask, osPriorityHigh, 0, 512);
//	WatchDogsTaskHandle = osThreadCreate(osThread(WatchDogsTask), NULL);


	osThreadDef(LEDUpTask, StartLEDUpTask, osPriorityLow, 0, 256);
	LEDUpTaskHandle = osThreadCreate(osThread(LEDUpTask), NULL);

}

void StartFutabaTask(void const * argument) {
	futaba.Init();
	for (;;) {
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
	steering_manager.init();
	for (;;) {
		steering_manager.proccess();
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
		osSignalSet(USBTaskHandle, USB_TX_signal);
	}
}


void StartOdometryTask(void const * argument) {
	odometry.Init();
	while (true) {
		osSignalWait(odometry.SignalReady, osWaitForever);
		odometry.Process(ahrs.attitude.values.yaw, motor.getDistance(), tools.GetMicros());
	}
}

void StartBatteryManager(void const * argument) {
	powermanager.Init();
	for (;;) {
		powermanager.Handler();
	}

}

void StartUSBTask(void const * argument) {
	USBLink::initHardware();
	while(true){
		usb_link.USB_Process();
	}
}

void StartTelemetryTask(void const * argument) {
	telemetry.Init();
	for (;;) {
		telemetry.Process();
	}

}

void StartBTTask(void const * argument) {
	Bluetooth_Init();
	for (;;) {
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
	WatchDogs::init();
	while(true){
		WatchDogs::process();
	}
}


void StartLEDUpTask(void const * argument){
	ledUp.Init();
	for(;;){
		ledUp.Process();
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
