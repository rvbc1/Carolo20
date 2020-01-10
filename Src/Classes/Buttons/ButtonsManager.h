/*
 * Buttons.h
 *
 *  Created on: 29.01.2019
 *      Author: Igor
 */

#ifndef CLASSES_BUTTONSMANAGER_H_
#define CLASSES_BUTTONSMANAGER_H_


#define MAX_BUTTON_AMOUNT    2

#include "cmsis_os.h"
#include "USBTask.h"
#include "Button.h"
#include "bitoperations.h"


extern uint8_t start_parking_USB;
extern uint8_t start_obstacle_USB;
extern uint8_t start_parking_sent;
extern uint8_t start_obstacle_sent;

class ButtonsManager {
private:
	Button *button_one;
	Button *button_two;
	//In case of 3 or more buttons, you must change type of flag and arguments in functions:
	// getData(), check(), updateFlag(), active(), activetedEver() and activatedFirst(),
	// and in bitoperations.h and .cpp (functions work on uint8_t)

	Button *all_buttons[MAX_BUTTON_AMOUNT];
	uint8_t button_number = 0;
	uint8_t buttonFlag = 0;
	uint8_t isEverActivated_reset = true;

	void addButton(Button *b);
	uint8_t getData();
	void check();

	// Update button flag functions
	void updateFlag(uint8_t);
	void activatedFirst(uint8_t);
	void active(uint8_t);
	void activatedEver(uint8_t);

public:
	void process();
	uint8_t readFlag(){ return buttonFlag; }
	void reset_activatedFirstFlag();
	void reset_activatedEverFlag();
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
