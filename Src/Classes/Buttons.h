/*
 * Buttons.h
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#ifndef CLASSES_BUTTONS_H_
#define CLASSES_BUTTONS_H_


#include "cmsis_os.h"
#include "USBTask.h"
#include "Button.h"

extern uint8_t start_parking_USB;
extern uint8_t start_obstacle_USB;
extern uint8_t start_parking_sent;
extern uint8_t start_obstacle_sent;

class Buttons {
private:
	Button *button_one;
	Button *button_two;

public:
	void process();
	uint8_t start1_state_of_pressing;
	uint8_t start2_state_of_pressing;
	uint8_t screen1_state_of_pressing;
	uint8_t screen2_state_of_pressing;
	uint8_t screen3_state_of_pressing;
	uint8_t any_button_was_pressed;
	void Init();
	Buttons();
	virtual ~Buttons();
};

extern Buttons buttons;

#endif /* CLASSES_BUTTONS_H_ */
