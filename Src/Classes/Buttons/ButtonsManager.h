/*
 * Buttons.h
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#ifndef CLASSES_BUTTONSMANAGER_H_
#define CLASSES_BUTTONSMANAGER_H_


#include "cmsis_os.h"
#include "USBTask.h"
#include "Button.h"



extern uint8_t start_parking_USB;
extern uint8_t start_obstacle_USB;
extern uint8_t start_parking_sent;
extern uint8_t start_obstacle_sent;

class ButtonsManager {
private:
	Button *button_one;
	Button *button_two;

	uint8_t ever1;
	uint8_t ever2;

	uint8_t first1;
	uint8_t first2;

	void check();
	//std::vector<Button *> buttons;
public:
	void process();
	uint8_t start1_state_of_pressing;
	uint8_t start2_state_of_pressing;
	uint8_t screen1_state_of_pressing;
	uint8_t screen2_state_of_pressing;
	uint8_t screen3_state_of_pressing;
	uint8_t any_button_was_pressed;
	uint8_t getState();
	void resert_buttons();
	void Init();
	ButtonsManager();
	virtual ~ButtonsManager();
};

extern ButtonsManager buttons_manager;

#endif /* CLASSES_BUTTONSMANAGER_H_ */
