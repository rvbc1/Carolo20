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
#include "bitoperations.h"
//#include <vector>

extern uint8_t start_parking_USB;
extern uint8_t start_obstacle_USB;
extern uint8_t start_parking_sent;
extern uint8_t start_obstacle_sent;

class ButtonsManager {
private:
	Button *button_one;
	Button *button_two;

	uint8_t buttonFlag = 0b00000000;

	void check();

	void updateFlag(uint8_t, uint8_t);
	void activatedFirst(uint8_t, uint8_t);
	void activatedEver(uint8_t, uint8_t);
	void active(uint8_t, uint8_t);
	//std::vector<Button *> buttons;
public:
	void process();
	uint8_t readFlag(){ return buttonFlag; }
	uint8_t start1_state_of_pressing;
	uint8_t start2_state_of_pressing;
	uint8_t screen1_state_of_pressing;
	uint8_t screen2_state_of_pressing;
	uint8_t screen3_state_of_pressing;
	uint8_t any_button_was_pressed;
	void Init();
	ButtonsManager();
	virtual ~ButtonsManager();
};

extern ButtonsManager buttons_manager;

#endif /* CLASSES_BUTTONSMANAGER_H_ */
