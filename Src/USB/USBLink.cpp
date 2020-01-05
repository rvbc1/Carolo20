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

void USBLink::USB_Process(void) {
	/* start -> code -> length -> DATA[length] -> length -> code -> stop */
	/* 6 + length */
	osEvent evt = osSignalWait(0, 500);
	if (evt.status == osEventSignal) {
		decodeRawData();
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
		recieveFrame();
	} else if (dataBuffer.rxSize == SETTINGS_RX_SIZE) {
		/* SETTINGS */
		//		USB_SetTransmission();
	} else if (dataBuffer.rxSize == COMMAND_RX_SIZE) {
		/* COMMAND */
		//		USB_ProcessCommand();
	} else if (dataBuffer.rxSize == TERMINAL_RX_SIZE) {
		/* Terminal */
		//TerminalFn();
	}
	usbRxRawSize = 0;
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
	case 't': {
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "ok!");
		break;
	}
	default: {
		dataBuffer.txSize = sprintf((char *) dataBuffer.tx.bytes, "not recognized :(");
		break;
	}
	CDC_Transmit_FS(dataBuffer.tx.bytes, dataBuffer.txSize);
	}
}

USBLink::USBLink() {
	frame_TX_SIZE = sizeof(FrameTX);
	frame_RX_SIZE = sizeof(FrameRX);
	dataBuffer.tx.bytes = new uint8_t [frame_TX_SIZE];
	//	dataBuffer.rx.bytes = new uint8_t [frame_RX_SIZE];

	initFrameTX();
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

