/*
 * DataLink.h
 *
 *  Created on: Jan 12, 2020
 *      Author: rvbc-
 */

#ifndef CLASSES_COMMUNICATION_DATALINK_H_
#define CLASSES_COMMUNICATION_DATALINK_H_

#include "main.h"

class DataLink {
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

    DataLink();
	virtual ~DataLink();

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

#endif /* CLASSES_COMMUNICATION_DATALINK_H_ */
