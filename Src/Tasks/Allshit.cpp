/*
 * Allshit.cpp
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */



#include <ButtonsManager.h>
#include "cmsis_os.h"
#include "Allshit.h"
#include "USBTask.h"
#include "Futaba.h"
#include "Gyro.h"
#include "PowerManager.h"
#include "AHRS.h"
#include "Motor.h"
#include "Tools.h"
#include "Telemetry.h"
#include "Buzzer.h"
#include "Servo.h"
#include "Bluetooth.h"
#include "Odometry.h"
#include "crc.h"
#include "OLED.h"
#include "Lights.h"
#include "ButtonsManager.h"
#include "Mathematics.h"
#include "tim.h"

#include "USBLink.h"

osThreadId GyroTaskHandle;
osThreadId AHRSTaskHandle;
osThreadId BatteryManagerHandle;
osThreadId SteeringTaskHandle;
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

void StartGyroTask(void const * argument);
void StartAHRSTask(void const * argument);
void StartOdometryTask(void const * argument);
void StartBatteryManager(void const * argument);
void StartSteeringTask(void const * argument);
void StartBTTask(void const * argument);
void StartFutabaTask(void const * argument);
void StartUSBTask(void const * argument);
void StartTelemetryTask(void const * argument);
void StartMotorController(void const * argument);
void StartBuzzerTask(void const * argument);
void StartOLEDTask(void const * argument);
void StartLightsTask(void const * argument);
void StartButtonsTask(void const * argument);

#include "iwdg.h"
#include "wwdg.h"

uint16_t cnt_blueled = 999;
uint8_t vision_reset = 0;
uint8_t vision_reset_sent = 0;
uint8_t vision_reset_ack = 0;



void Allshit_begin(void) {

	/* definition and creation of FutabaTask */
	osThreadDef(FutabaTask, StartFutabaTask, osPriorityHigh, 0, 256);
	FutabaTaskHandle = osThreadCreate(osThread(FutabaTask), NULL);

	/* definition and creation of MotorController */
	osThreadDef(MotorController, StartMotorController, osPriorityHigh, 0, 512);
	MotorControllerHandle = osThreadCreate(osThread(MotorController), NULL);

	/* definition and creation of SteeringTask */
	osThreadDef(SteeringTask, StartSteeringTask, osPriorityHigh, 0, 512);
	SteeringTaskHandle = osThreadCreate(osThread(SteeringTask), NULL);

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
}

void StartFutabaTask(void const * argument) {
	futaba.Init();
	for (;;) {
		futaba.Process();
	}

}

void StartMotorController(void const * argument) {
	motor.Init();
	while(1) {
		motor.Process();
		osSignalSet(OdometryTaskHandle, odometry.SignalReady);
	}
}

static void StickCommandProccess(void) {
	if (futaba.Stick_Command[1]) // (   .)    (   .)
		motor.SetPassthroughState(true);
	else
		motor.SetPassthroughState(false);
	if (futaba.Stick_Command[0]) // (.   )    (.   )
		motor.setMaxVelocity(6000.f);
	else
		motor.setMaxVelocity(3500.f);

	static bool last_cmd = false;
	if (futaba.Stick_Command[4] != last_cmd) { // (.   )    (   .)   < - to nie sa cycki
		last_cmd = futaba.Stick_Command[4];
		gyro.StartCalibration();
		odometry.Reset(ahrs.attitude.values.yaw, motor.getDistance(),tools.GetMicros());
		odometry.SetCurrentPosition();
	}
}
void StartSteeringTask(void const * argument) {
	enum {
		DISARMED = 0,
		MODE_AUTONOMOUS,
		MODE_SEMI,
		MODE_ACRO
	} rc_mode = DISARMED;

	const uint32_t task_dt = 1u;

	futaba.ConfigureSmoothing(50.f, task_dt * 1e-3); /* Nyquist frequency - 1/2 Radio frequency * 0.9; 8CH - 9ms, 16CH - 18ms,*/

	MX_TIM2_Init();
	osDelay(200);
	HAL_TIM_PWM_Start(&htim2 , TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,   TIM_CHANNEL_4);

	Servo* servo_back;
	Servo* servo_front;
	servo_back = new Servo(&TIM2->CCR2);
	servo_front = new Servo(&TIM2->CCR4);
	//	servo_front.Init();
	//	servo_back.Init();

	uint8_t reczny = 0;
	osDelay(200);
	motor.SetPassthroughState(false);
	motor.setMaxVelocity(3500.f);
	osDelay(100);

	for (;;) {
		futaba.ProcessSmoothing();

		if (futaba.Get_RCState() || futaba.SwitchA < SWITCH_DOWN) {
			rc_mode = DISARMED;
			left_indicator = 1;
			right_indicator = 1;

			//			servo_front.Disarm();
			//			servo_back.Disarm();
			motor.Disarm();
			if (futaba.Get_RCState() == 0)
				StickCommandProccess();
		} else if (futaba.SwitchA == SWITCH_DOWN) {
			if (futaba.SwitchC == SWITCH_UP) {
				left_indicator = 0;
				right_indicator = 1;
			} else if(futaba.SwitchC == SWITCH_MIDDLE){
				left_indicator = 0;
				right_indicator = 0;
			} else{
				left_indicator = 1;
				right_indicator = 0;
			}

			if (futaba.SwitchB == SWITCH_UP) {

				//				if (reczny == true){
				//					HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_SET);
				//					motor.SetVelocity(0, 10000.f, 50000.f);
				//					osDelay(1000);
				//				}
				reczny = false;
				rc_mode = MODE_ACRO;


				servo_back->setAngle(-int16_t(futaba.SmoothDeflection[YAW] * 90.f) + 90);
				servo_front->setAngle(int16_t(futaba.SmoothDeflection[YAW] * 90.f) + 90);
				//servo_front.SetAngleD(futaba.SmoothDeflection[YAW] * 45.f, 0.f);
				//servo_back.SetAngleD(futaba.SmoothDeflection[YAW] * 45.f, 0.f); // przy recznym po prostu z minusem
				motor.SetDuty(futaba.SmoothDeflection[PITCH]);
				motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 10000.f, 50000.f);

			} else if (futaba.SwitchB == SWITCH_MIDDLE) {
				reczny = true;
				rc_mode = MODE_SEMI;


				//			servo_front.SetAngleD(odroid_setpoints.fi, odroid_setpoints.dfi);
				//				servo_back.SetAngleD(odroid_setpoints.fi, odroid_setpoints.dfi); // trzeba dac inny parametr jak ma byc niezaleznie
				motor.SetDuty(futaba.SmoothDeflection[PITCH]);
				motor.SetVelocity(motor.getMaxVelocity() * futaba.SmoothDeflection[PITCH], 3000.f, 50000.f);
			} else if (futaba.SwitchB == SWITCH_DOWN) {
				reczny = true;
				rc_mode = MODE_AUTONOMOUS;

				//			servo_front.SetAngleD(odroid_setpoints.fi, odroid_setpoints.dfi);
				//				servo_back.SetAngleD(odroid_setpoints.fi, odroid_setpoints.dfi);
				motor.SetVelocity(odroid_setpoints.velocity, odroid_setpoints.acceleration, odroid_setpoints.jerk);
			}
			//		TIM2->CCR2 = 1450;
			//		servo_front.PositionTracking();
			//		servo_back.PositionTracking();
			motor.Arm();
		}

		if (futaba.SwitchB == SWITCH_UP) {

			//HAL_TIM_Base_Stop_IT(&htim11);
			TIM11->CNT = 1;

			cnt_blueled++;
			if (cnt_blueled > 1000)
			{
				HAL_GPIO_TogglePin(LED_OUT_GPIO_Port, LED_OUT_Pin);
				cnt_blueled = 1;
			}

		} else if (futaba.SwitchB == SWITCH_MIDDLE) {
			//HAL_TIM_Base_Start_IT(&htim11);
			TIM11->CNT = 1;
			cnt_blueled = 999;
			HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_RESET);
		} else if (futaba.SwitchB == SWITCH_DOWN) {
			//HAL_TIM_Base_Start_IT(&htim11);
			TIM11->CNT = 1;
			cnt_blueled = 999;
			HAL_GPIO_WritePin(LED_OUT_GPIO_Port, LED_OUT_Pin, GPIO_PIN_RESET);
		}

		if(futaba.SwitchB == SWITCH_UP){
			vision_reset_ack = 0;
			vision_reset_sent = 0;
			vision_reset = 0;
		} else if(futaba.SwitchB == SWITCH_DOWN){
			if(vision_reset_sent == 0){
				vision_reset = 1;
				vision_reset_ack = 1;
			}else {
				vision_reset = 0;
			}
		}


		//TODO - Find best suited place for watchdog refreshes
		static uint8_t watchdog_init_done = 0;
		if (watchdog_init_done) {
			HAL_WWDG_Refresh(&hwwdg);
			HAL_IWDG_Refresh(&hiwdg);
		} else if(HAL_GetTick() > 500) {
			MX_IWDG_Init();
			MX_WWDG_Init();
			watchdog_init_done = 1;
		}

		osDelay(task_dt);
	}
	UNUSED(rc_mode);
}

void StartGyroTask(void const * argument) {
	gyro.Init();
	while(1) {
		gyro.Process();
	}
}

void StartAHRSTask(void const * argument) {
	ahrs.Init();
	while(1) {
		ahrs.Process();
		osSignalSet(USBTaskHandle, USB_TX_signal);
	}
}


void StartOdometryTask(void const * argument) {
	odometry.Init();
	while (1) {
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
	//USB_Init();

	while(1)
	{
		usb_link.USB_Process();
		//osDelay(5);
		//USB_Process();
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
	while(1)
	{
		buzzer.Loop();
	}
}

void StartOLEDTask(void const * argument){
	oled.Init();
	while(1)
	{
		oled.process();
	}
}


void StartLightsTask(void const * argument){
	lights.ws2812_init();
	while(1)
	{
		lights.process();
	}
}


void StartButtonsTask(void const * argument){
	buttons_manager.Init();
	while(1){
		buttons_manager.process();

		osDelay(5);
	}
}
