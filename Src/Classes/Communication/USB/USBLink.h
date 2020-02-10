/*
 * USBLink.h
 *
 *  Created on: 03.01.2020
 *      Author: rvbc-
 */

#ifndef USB_USBLINK_H_
#define USB_USBLINK_H_

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "Mathematics.h"
#include "crc.h"


//#include "USBTask.h" ///
#include "tim.h"
#include "../../../Tasks&Callbacks/AllTasks.h"






#define START_CODE 0xff
#define END_CODE 0xfe

#define SETTINGS_RX_SIZE 4

#define COMMAND_RX_SIZE 3

#define TERMINAL_RX_SIZE 1

extern int32_t USB_TX_signal;

typedef struct setpoints_from_vision_s{
	float fi_front;
	float fi_back;
	float dfi;
	float velocity;
	float acceleration;
	float jerk;
	uint8_t left_inidcator;
	uint8_t right_inidcator;
} setpoints_from_vision_t;

extern setpoints_from_vision_t setpoints_from_vision;



class USBLink {
public:
    struct ValuesTX{

        uint32_t timecode;

        int32_t distance;
        int16_t velocity;
        int16_t w, x, y, z;
        uint16_t yaw;
        int16_t rates[3];
        int16_t acc[3];

        uint8_t buttons;
        uint8_t lights;
        uint8_t futabastate;

    } __attribute__ ((__packed__));


    struct ValuesRX{

        uint32_t timecode;

        int16_t steering_fi_front;
        int16_t steering_fi_back;

        int16_t speed;
        int16_t acceleration;
        int16_t jerk;


    } __attribute__ ((__packed__));


    void USB_Process();


    static int8_t MAIN_USB_Receive(uint8_t* Buf, uint32_t *Len);

    static void initHardware();

	USBLink();
	virtual ~USBLink();

private:
	struct FrameTX{
		uint8_t start_code;

        uint8_t length;

		ValuesTX values;

        uint16_t crc16;

		uint8_t end_code;
	} __attribute__ ((__packed__));

	struct FrameRX{
		uint8_t start_code;

        uint8_t length;

		ValuesRX values;

        uint16_t crc16;

		uint8_t end_code;
	} __attribute__ ((__packed__));

	struct SettingsRX{
		uint8_t start_code;

		uint8_t code;

		uint8_t data;

		uint8_t end_code;
	} __attribute__ ((__packed__));

	struct CommandRX{
		uint8_t start_code;

		uint8_t command;

		uint8_t end_code;
	} __attribute__ ((__packed__));



	union DataTX{
		FrameTX* frame;
		uint8_t* bytes;
	};

	union DataRX{
		SettingsRX* settings_frame;
		CommandRX* command_frame;
		FrameRX* frame;
		uint8_t* bytes;
	};

	struct DataBuffer{
		DataTX tx;
		DataRX rx;
		uint16_t txSize;
		uint16_t rxSize;
	};

	static DataBuffer dataBuffer;

    void initFrameTX();

    void decodeRawData();

    void recieveFrame();

    void recieveSettings();

    void recieveCommand();

    void recieveTerminal();

    void transmitFrame();

    void prepareFrameTX();


    uint8_t checkFrameCorrectness(FrameRX* frame);
    uint8_t checkFrameCorrectness(SettingsRX* frame);
    uint8_t checkFrameCorrectness(CommandRX* frame);

	uint8_t is_sending_data;
	uint8_t is_updating_data;
	uint8_t CommunicationOnGoing = false;

	uint16_t frame_TX_SIZE;
	uint16_t frame_RX_SIZE;
	uint16_t frame_TX_values_size;
	uint16_t frame_RX_values_size;

};

extern USBLink usb_link;

#endif /* USB_USBLINK_H_ */
