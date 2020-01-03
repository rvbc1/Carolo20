/*
 * USBLink.h
 *
 *  Created on: 03.01.2020
 *      Author: Igor
 */

#ifndef USB_USBLINK_H_
#define USB_USBLINK_H_

#include "main.h"

#define START_BYTE 0xff
#define END_BYTE 0xfe

class USBLink {
public:
    struct ValuesTX{

        uint8_t code;
        uint8_t length;

        uint32_t timecode;

        int32_t distance;
        int16_t velocity;
        int16_t w, x, y, z;
        uint16_t yaw;
        int16_t rates[3];
        int16_t acc[3];

        uint8_t startbutton1;
        uint8_t startbutton2;
        uint8_t visionrst;
        uint8_t futabastate;

    } __attribute__ ((__packed__));

    struct ValuesRX{

        uint8_t code;
        uint8_t length;

        uint32_t timecode;

        int16_t steering_fi;
        int16_t steering_dfi;

        int16_t speed;
        int16_t acceleration;
        int16_t jerk;

        uint8_t left_indicator_u;
        uint8_t right_indicator_u;

    } __attribute__ ((__packed__));


    void initFrameTX();


	USBLink();
	virtual ~USBLink();

private:
	struct FrameTX{
		uint8_t start_code;

		ValuesTX values;

		uint8_t end_code;
	} __attribute__ ((__packed__));

	struct FrameRX{
		uint8_t start_code;

		ValuesRX values;

		uint8_t end_code;
	} __attribute__ ((__packed__));

	union{
		FrameTX* frame;
		uint8_t* bytes;
	}dataTX;

	union{
		FrameRX* frame;
		uint8_t* bytes;
	}dataRX;

	uint8_t is_sending_data;
	uint8_t is_updating_data;

	uint16_t frame_TX_SIZE;
	uint16_t frame_RX_SIZE ;
};

#endif /* USB_USBLINK_H_ */
