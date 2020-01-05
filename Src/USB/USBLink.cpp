/*
 * USBLink.cpp
 *
 *  Created on: 03.01.2020
 *      Author: rvbc-
 */

#include <USBLink.h>


uint8_t* USBLink::usbRxRawBuffer = 0;
uint16_t USBLink::usbRxRawSize = 0;

USBLink::DataBuffer USBLink::dataBuffer;

extern int32_t USB_TX_signal;

extern USBD_HandleTypeDef hUsbDeviceFS;

int32_t USB_RX_signal2 = 1 << 0;

USBLink usb_link;

extern void MX_USB_DEVICE_Init(void);

void USBLink::USB_Process(void) {
	/* start -> code -> length -> DATA[length] -> length -> code -> stop */
	/* 6 + length */
	osEvent evt = osSignalWait(0, 500);
	if (evt.status == osEventSignal) {
		if (evt.value.signals & USB_RX_signal2) {
			decodeRawData();
		}
		//	if (evt.value.signals & USB_TX_signal && CommunicationOnGoing && hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
		//		static uint8_t cnt = 0;
		//		if (++cnt >= usbDenominator) {
		//			cnt = 0;
		//		USB_Transmit_Data();
		//		}
		//			TIM11->CNT = 0;
		//		}
		//	if (evt.value.signals & USB_RX_signal) {


		//	}
		//			static uint16_t cnt = 0;
		//			if (cnt++ > 500) {
		//				HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		//				cnt = 0;
		//			}
	} else if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
		//			HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	}
}

void USBLink::decodeRawData(){
	if (dataBuffer.rxSize == frame_RX_SIZE) {
		/* DATA */
		//		USB_Receive_Data();
		//recieveFrame();
	} else if (dataBuffer.rxSize == SETTINGS_RX_SIZE) {
		/* SETTINGS */
		//		USB_SetTransmission();
	} else if (dataBuffer.rxSize == COMMAND_RX_SIZE) {
		/* COMMAND */
		//		USB_ProcessCommand();
	} else if (dataBuffer.rxSize == TERMINAL_RX_SIZE) {
		/* Terminal */
		//TerminalFn();
		recieveTerminal();
	}
	usbRxRawSize = 0;
	dataBuffer.rxSize = 0;
}

void USBLink::recieveFrame(){
	FrameRX* frame = dataBuffer.rx.frame;//Just for shorter code
	if(frame->start_code == START_CODE && frame->end_code == END_CODE){
		odroid_setpoints.fi = (float)(frame->values.steering_fi * 18.f / 1000.f / M_PI_FLOAT);
		odroid_setpoints.dfi = (float)(frame->values.steering_dfi * 18.f / 1000.f / M_PI_FLOAT);

		odroid_setpoints.velocity = (float)(frame->values.speed);
		odroid_setpoints.acceleration = (float)(frame->values.acceleration);
		odroid_setpoints.jerk = (float)(frame->values.jerk);
	}
}

void USBLink::recieveTerminal(){
	switch(dataBuffer.rx.bytes[0]){
	case 's':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "Task:\t\t\t\tTick:\t\tRun Time %%\n");
		vTaskGetRunTimeStats((char*) dataBuffer.tx.bytes + dataBuffer.txSize);
		dataBuffer.txSize = strlen((char*) dataBuffer.tx.bytes);
		dataBuffer.txSize += sprintf((char *) dataBuffer.tx.bytes+dataBuffer.txSize, "-------\n");;
		break;
	case 'B':
//		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nLi-Po:\t%.2fV\nCurrent:\t%.3fA\nAN_IN:\t%.2fV\nSTM Temperature:%.2fC\n", powermanager.voltage,
//				powermanager.amperage, powermanager.analog_in, powermanager.temperature);
		break;
	case 'e':
	//	dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "Current spd: %.1f\tSetspeed: %.1f\nTotalCount: %ld\tTotalRoad: %.1f\n\n", motor.getVelocity(), motor.getSetVelocity(), motor.getImpulses(), motor.getDistance());
		break;
	case 'f':
	//	dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "A: %d\tE: %d\tT: %d\tR: %d\nSwitchA: %d\tSwitchB: %d\n", futaba.sbusChannelData[0], futaba.sbusChannelData[1],
	//			futaba.sbusChannelData[2], futaba.sbusChannelData[3], futaba.SwitchA, futaba.SwitchB);
		break;
	case 'F':
	//	dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "A: %f\tE: %f\tT: %f\tR: %f\n", futaba.StickDeflection[ROLL], futaba.StickDeflection[PITCH],
	//			futaba.StickDeflection[THROTTLE], futaba.StickDeflection[YAW]);
		break;
	case 'G':
	//	dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nrates:\t%.2f\t%.2f\t%.2f\nangles:\t%.2f\t%.2f\t%.2f\tFLAT: %.2f\ntemperature:\t%.1fC\naccels:\t%.2fG\t%.2fG\t%.2fG\n",
	//			gyro.rates[0], gyro.rates[1], gyro.rates[2], ahrs.attitude.values.roll, ahrs.attitude.values.pitch, ahrs.attitude.values.yaw, gyro.angles[2], gyro.temperature,
//				gyro.g_rates[0], gyro.g_rates[1], gyro.g_rates[2]);
		break;
	case 'q':
	//	quaternion orientation;
	//	ahrs.getQuaternion(&orientation);
	//	dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "\nw = %.3f\tx = %.3f\ty = %.3f\tz = %.3f\n",
//				orientation.w, orientation.x, orientation.y, orientation.z);
		break;
	case 'g':
	//	gyro.StartCalibration();
	//	odometry.Reset(ahrs.attitude.values.yaw, motor.getDistance(),tools.GetMicros());
	//	odometry.SetCurrentPosition();
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "Gyro recalibrating . . .\n\r");
		break;
	case 'M':
	//	dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "velocity: %.1f\ndistance: %.1f\n\n", motor.getVelocity(), motor.getDistance());
		break;
	case 'o':
//		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "x: %.1f\ty: %.1f\nVx: %.1f\tVy: %.1f\n\n", odometry.getX(), odometry.getY(), odometry.getVx(), odometry.getVy());
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
		//CommunicationOnGoing = !CommunicationOnGoing;
		dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "Switched to Terminal or ROS Mode\n");
		break;
	case 't':
	//	dataBuffer.txSize = sprintf((char*) dataBuffer.tx.bytes, "time: %lums / %luus\n",HAL_GetTick(), tools.GetMicros());
		break;
	case 'u':
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "USBServo: %.1f\tUSBVelocity: %.1f\n", odroid_setpoints.fi, odroid_setpoints.velocity);
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
	default:
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "not recognized :(");
		break;
	}
	CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
}

USBLink::USBLink() {
	frame_TX_SIZE = sizeof(FrameTX);
	frame_RX_SIZE = sizeof(FrameRX);
	//dataBuffer.tx.bytes = new uint8_t [frame_TX_SIZE];
	dataBuffer.tx.bytes = new uint8_t [100];
	dataBuffer.rxSize = 0;
	//	dataBuffer.rx.bytes = new uint8_t [frame_RX_SIZE];

	initFrameTX();

	//	MX_USB_DEVICE_Init();
	//	MX_TIM11_Init();
}

void USBLink::initFrameTX(){
	dataBuffer.tx.frame->start_code = START_CODE;
	dataBuffer.tx.frame->end_code = END_CODE;
}

int8_t USBLink::MAIN_USB_Receive(uint8_t* Buf, uint32_t *Len) {
	dataBuffer.rx.bytes = Buf;
	dataBuffer.rxSize = *Len;
	osSignalSet(USBTaskHandle, USB_RX_signal2);
	return 0;
}

USBLink::~USBLink() {
	// TODO Auto-generated destructor stub
}

