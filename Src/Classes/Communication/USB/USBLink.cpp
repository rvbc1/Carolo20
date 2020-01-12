/*
 * USBLink.cpp
 *
 *  Created on: 03.01.2020
 *      Author: rvbc-
 */

#include <USBLink.h>

#include "Futaba.h"
#include "Telemetry.h"
#include "Motor.h"
#include "Gyro.h"
#include "Tools.h"
#include "AHRS.h"
#include "PowerManager.h"
#include "Buzzer.h"
#include "Odometry.h"
#include <ButtonsManager.h>
#include <LightsManager.h>
#include "Lights/Light.h"

USBLink::DataBuffer USBLink::dataBuffer;


extern USBD_HandleTypeDef hUsbDeviceFS;

//extern int32_t USB_TX_signal;
//
//extern int32_t USB_RX_signal;
//
//
//extern uint8_t usbDenominator;

int32_t USB_RX_signal = 1 << 0;
int32_t USB_TX_signal = 1 << 1;
uint8_t usbDenominator = 5;

uint32_t timestamp = 5;

setpoints_from_vision_t setpoints_from_vision = {0.f, 0.f, 0.f, 0.f, 0.f};


USBLink usb_link;


void USBLink::USB_Process(void) {
	/* start -> code -> length -> DATA[length] -> length -> code -> stop */
	/* 6 + length */
	osEvent evt = osSignalWait(0, 500);
	if (evt.status == osEventSignal) {
		if (evt.value.signals & USB_TX_signal && CommunicationOnGoing && hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
			static uint8_t cnt = 0;
			if (++cnt >= usbDenominator) {// Ograniczenie predkosci transmisji
				cnt = 0;
				transmitFrame();
			}
			TIM11->CNT = 0; // Restars ala watchdoga
		}

		if (evt.value.signals & USB_RX_signal) {
			decodeRawData();
		}

	} else if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
		//			HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	}
}

void USBLink::transmitFrame(){
	FrameTX* frame = dataBuffer.tx.frame;//Just for shorter code

	frame->values.timecode = HAL_GetTick();

	frame->values.distance = motor.getDistance();
	frame->values.velocity = motor.getVelocity();

	quaternion orientation;
	ahrs.getQuaternion(&orientation);
	frame->values.w = (int16_t)(orientation.w * 32767.f);
	frame->values.x = (int16_t)(orientation.x * 32767.f);
	frame->values.y = (int16_t)(orientation.y * 32767.f);
	frame->values.z = (int16_t)(orientation.z * 32767.f);

	frame->values.yaw = (uint16_t)(gyro.angles[2]/360.f*65536.f);

	for(uint8_t axis = 0; axis < 3; axis++){
		frame->values.rates[axis] = (int16_t)(gyro.rates[axis] * gyro.getGyroScale());
		frame->values.acc[axis] = (int16_t)(gyro.g_rates[axis] * gyro.getAccScale());
	}

	frame->values.buttons = buttons_manager.getData();
	frame->values.futabastate = futaba.SwitchB;

	prepareFrameTX();

	CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
}

void USBLink::prepareFrameTX(){
	FrameTX* frame = dataBuffer.tx.frame;//Just for shorter code
	frame->start_code = START_CODE;

	frame->length = frame_TX_SIZE;
	frame->crc16 = crc16((uint8_t *) &frame->values, frame_TX_values_size);
	frame->end_code = END_CODE;

	dataBuffer.txSize = frame_TX_SIZE;
}

void USBLink::decodeRawData(){
	if (dataBuffer.rxSize == frame_RX_SIZE) {
		recieveFrame();
	} else if (dataBuffer.rxSize == SETTINGS_RX_SIZE) {
		recieveSettings();
	} else if (dataBuffer.rxSize == COMMAND_RX_SIZE) {
		recieveCommand();
	} else if (dataBuffer.rxSize == TERMINAL_RX_SIZE) {
		recieveTerminal();
	}
	dataBuffer.rxSize = 0;
}

void USBLink::recieveFrame(){
	FrameRX* frame = dataBuffer.rx.frame;//Just for shorter code
	if (checkFrameCorrectness(frame)) {
		setpoints_from_vision.fi_front = (float)(frame->values.steering_fi_front * 18.f / 1000.f / M_PI_FLOAT);
		setpoints_from_vision.fi_back = (float)(frame->values.steering_fi_back * 18.f / 1000.f / M_PI_FLOAT);

		setpoints_from_vision.velocity = (float)(frame->values.speed);
		setpoints_from_vision.acceleration = (float)(frame->values.acceleration);
		setpoints_from_vision.jerk = (float)(frame->values.jerk);
	}
}

uint8_t USBLink::checkFrameCorrectness(FrameRX* frame){
	if(frame->start_code == START_CODE && frame->length == frame_RX_SIZE && frame->end_code == END_CODE)
		return true;
	return false;
}

void USBLink::recieveSettings(){
	SettingsRX* frame = dataBuffer.rx.settings_frame;//Just for shorter code
	if (checkFrameCorrectness(frame)) {
		usbDenominator = frame->data;
	}
}

uint8_t USBLink::checkFrameCorrectness(SettingsRX* frame){
	if (frame->start_code == START_CODE && frame->code == 0xBE && frame->end_code == END_CODE)
		return true;
	return false;
}

void USBLink::recieveCommand(){
	CommandRX* frame = dataBuffer.rx.command_frame;//Just for shorter code
	if (checkFrameCorrectness(frame)) {
		switch (frame->command) {

		case 0x01:
			CommunicationOnGoing = true;
			break;
		case 0x02:
			CommunicationOnGoing = false;
			break;
		case 0x10:
			gyro.StartCalibration();
			odometry.Reset(ahrs.attitude.values.yaw, motor.getDistance(),tools.GetMicros());
			odometry.SetCurrentPosition();
			break;
		case 0x20:

			break;

		case 0x30:

			break;

		case 0x50:
			headlights.setActivated(true);
			break;

		case 0x51:
			headlights.setActivated(false);
			break;

		case 0x52:
			headlights.setColor(low_beam_color);
			break;

		case 0x53:
			headlights.setColor(high_beam_color);
			break;

		case 0x54:
			tail_lights.setActivated(true);
			break;

		case 0x55:
			tail_lights.setActivated(false);
			break;

		case 0x56:
			break_lights.setActivated(true);
			break;

		case 0x57:
			break_lights.setActivated(false);
			break;

		case 0x58:
			left_indicator_front.setActivated(true);
			left_indicator_back.setActivated(true);
			break;

		case 0x59:
			left_indicator_front.setActivated(false);
			left_indicator_back.setActivated(false);
			break;

		case 0x5A:
			right_indicator_front.setActivated(true);
			right_indicator_back.setActivated(true);
			break;

		case 0x5B:
			right_indicator_front.setActivated(false);
			right_indicator_back.setActivated(false);
			break;

		case 0x60:
			buttons_manager.reset();
			break;

		case 0xDE:
			systemResetToBootloader();
			break;
		case 0xAD:
			NVIC_SystemReset();
			break;
		}
	}

}

uint8_t USBLink::checkFrameCorrectness(CommandRX* frame){
	if (frame->start_code == START_CODE && frame->end_code == END_CODE)
		return true;
	return false;
}


void USBLink::recieveTerminal(){
	switch(dataBuffer.rx.bytes[0]){
	case 's':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "Task:\t\tTick:\t\tRun Time %%\n");
		vTaskGetRunTimeStats((char*) dataBuffer.tx.bytes + dataBuffer.txSize);
		dataBuffer.txSize = strlen((char*) dataBuffer.tx.bytes);
		dataBuffer.txSize += sprintf((char *) dataBuffer.tx.bytes+dataBuffer.txSize, "-------\n");;
		break;
	case 'b':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "Buttons value: %d\n", buttons_manager.getData());

	break;

	case 'B':
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nLi-Po:\t%.2fV\nCurrent:\t%.3fA\nAN_IN:\t%.2fV\nSTM Temperature:%.2fC\n", powermanager.voltage,
				powermanager.amperage, powermanager.analog_in, powermanager.temperature);
		break;
	case 'e':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "Current spd: %.1f\tSetspeed: %.1f\nTotalCount: %ld\tTotalRoad: %.1f\n\n", motor.getVelocity(), motor.getSetVelocity(), motor.getImpulses(), motor.getDistance());
		break;
	case 'f':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "A: %d\tE: %d\tT: %d\tR: %d\nSwitchA: %d\tSwitchB: %d\n", futaba.sbusChannelData[0], futaba.sbusChannelData[1],
				futaba.sbusChannelData[2], futaba.sbusChannelData[3], futaba.SwitchA, futaba.SwitchB);
		break;
	case 'F':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "A: %f\tE: %f\tT: %f\tR: %f\n", futaba.StickDeflection[ROLL], futaba.StickDeflection[PITCH],
				futaba.StickDeflection[THROTTLE], futaba.StickDeflection[YAW]);
		break;
	case 'G':
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nrates:\t%.2f\t%.2f\t%.2f\nangles:\t%.2f\t%.2f\t%.2f\tFLAT: %.2f\ntemperature:\t%.1fC\naccels:\t%.2fG\t%.2fG\t%.2fG\n",
				gyro.rates[0], gyro.rates[1], gyro.rates[2], ahrs.attitude.values.roll, ahrs.attitude.values.pitch, ahrs.attitude.values.yaw, gyro.angles[2], gyro.temperature,
				gyro.g_rates[0], gyro.g_rates[1], gyro.g_rates[2]);
		break;
	case 'h':
		headlights.setActivated(!headlights.getActivated());
		break;
	case 'H':
		lights_manager.high = !lights_manager.high;
		break;
	case 'q':
		quaternion orientation;
		ahrs.getQuaternion(&orientation);
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nw = %.3f\tx = %.3f\ty = %.3f\tz = %.3f\n",
				orientation.w, orientation.x, orientation.y, orientation.z);
		break;
	case 'g':
		gyro.StartCalibration();
		odometry.Reset(ahrs.attitude.values.yaw, motor.getDistance(),tools.GetMicros());
		odometry.SetCurrentPosition();
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "Gyro recalibrating . . .\n\r");
		break;
	case 'M':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "velocity: %.1f\ndistance: %.1f\n\n", motor.getVelocity(), motor.getDistance());
		break;
	case 'o':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "x: %.1f\ty: %.1f\nVx: %.1f\tVy: %.1f\n\n", odometry.getX(), odometry.getY(), odometry.getVx(), odometry.getVy());
		break;
	case 'O':
		//	TIM3->CNT += -1500;
		//	dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "dist: %.1f\n\n", motor.getDistance());
		break;
	case 'm':
		//						len = sprintf((char *) usbTxBuffer, "time:\tset_pwm:\trpms:\n");
		//						for(int index = 0; index < 8000; index = index + 8) {
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index], motor_logging1[index], motor_logging2[index]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+1], motor_logging1[index+1], motor_logging2[index+1]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+2], motor_logging1[index+2], motor_logging2[index+2]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+3], motor_logging1[index+3], motor_logging2[index+3]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+4], motor_logging1[index+4], motor_logging2[index+4]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+5], motor_logging1[index+5], motor_logging2[index+5]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+6], motor_logging1[index+6], motor_logging2[index+6]);
		//							len += sprintf((char *) usbTxBuffer + len, "%.6f\t%.1f\t%.2f\n", motor_logging0[index+7], motor_logging1[index+7], motor_logging2[index+7]);
		//							CDC_Transmit_FS(usbTxBuffer, len);
		//							len = 0;
		//							osDelay(1);
		//						}
		break;
	case 'T':
		CommunicationOnGoing = !CommunicationOnGoing;
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "Switched to Terminal or ROS Mode\n");
		break;
	case 't':
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "time: %lums / %luus\n",HAL_GetTick(), tools.GetMicros());
		break;
	case 'u':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "USBServo: %.1f\tUSBVelocity: %.1f\n", setpoints_from_vision.fi_front, setpoints_from_vision.velocity);
		break;
	case 'r':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "\n...STM32 Resetting ..\n");
		CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
		NVIC_SystemReset();
		break;
	case 'R':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "\n...Jumping to System Memory Bootloader ..\n");
		CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
		systemResetToBootloader();
		break;
	case 'l':
//		left_indicator = !left_indicator;
//		right_indicator = !right_indicator;
		break;
	case 'L':

		break;
	default:
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "\n%c - not recognized :(\n", dataBuffer.rx.bytes[0]);
		break;
	}
	CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
}

USBLink::USBLink() {
	frame_TX_SIZE = sizeof(FrameTX);
	frame_RX_SIZE = sizeof(FrameRX);
	frame_TX_values_size = sizeof(ValuesTX);
	frame_RX_values_size = sizeof(ValuesRX);
	//dataBuffer.tx.bytes = new uint8_t [frame_TX_SIZE];
	dataBuffer.tx.bytes = new uint8_t [1024];
	dataBuffer.rxSize = 0;
	//	dataBuffer.rx.bytes = new uint8_t [frame_RX_SIZE];

	initFrameTX();
}

void USBLink::initFrameTX(){
	dataBuffer.tx.frame->start_code = START_CODE;
	dataBuffer.tx.frame->end_code = END_CODE;
}

void USBLink::initHardware(){
	MX_USB_DEVICE_Init();
	MX_TIM11_Init();
}

int8_t USBLink::MAIN_USB_Receive(uint8_t* Buf, uint32_t *Len) {
	dataBuffer.rx.bytes = Buf;
	dataBuffer.rxSize = *Len;
	osSignalSet(USBTaskHandle, USB_RX_signal);
	return 0;
}

USBLink::~USBLink() {
	// TODO Auto-generated destructor stub
}
