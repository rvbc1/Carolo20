/*
 * USBLink.cpp
 *
 *  Created on: 03.01.2020
 *      Author: Igor
 */

#include <USBLink.h>

USBLink::USBLink() {
	frame_TX_SIZE = sizeof(FrameTX);
	frame_RX_SIZE = sizeof(FrameRX);
	dataTX.bytes = new uint8_t [frame_TX_SIZE];
	dataRX.bytes = new uint8_t [frame_RX_SIZE];

	initFrameTX();
}

USBLink::initFrameTX(){
	dataTX.frame->start_code = STAT_BYTE;
	dataTX.frame->end_code = END_BYTE;
}

USBLink::~USBLink() {
	// TODO Auto-generated destructor stub
}

