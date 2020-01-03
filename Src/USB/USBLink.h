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
    struct ValuesRX{
        uint16_t ele_angle;

        uint16_t SERVO_1;

        uint16_t SERVO_2;

        uint16_t SERVO_3;

        uint16_t SERVO_4;

    } __attribute__ ((__packed__));

    struct ValuesTX{
        uint16_t ele_angle;

        uint16_t enc1_raw;

        uint16_t enc1_angle;

        uint16_t enc2_raw;

        uint16_t enc2_angle;

        uint16_t enc3_raw;

        uint16_t enc3_angle;

        uint16_t enc4_raw;

        uint16_t enc4_angle;

        uint16_t batt;
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
