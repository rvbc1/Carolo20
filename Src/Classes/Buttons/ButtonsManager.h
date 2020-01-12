/*
 * Buttons.h
 *
 *  Created on: 29.01.2019
 *      Author: rvbc-
 */

#ifndef CLASSES_BUTTONSMANAGER_H_
#define CLASSES_BUTTONSMANAGER_H_

#define ALL_BUTTONS_FLAGS_DATA_TYPE 	uint8_t
#define MAX_BUTTONS_AMOUNT    			2

#include "main.h"
#include "cmsis_os.h"
#include "Button.h"
#include "bitoperations.h"





class ButtonsManager {
private:
	const uint32_t task_dt = 5u;
	Button *button_one;
	Button *button_two;
	//In case of 3 or more buttons, you must change type of flag and arguments in functions:
	// getData(), check(), updateFlag(), active(), activetedEver() and activatedFirst(),
	// and in bitoperations.h and .cpp (functions work on uint8_t)

	Button *all_buttons[MAX_BUTTONS_AMOUNT];
	uint8_t amount_of_added_buttons = 0;

	ALL_BUTTONS_FLAGS_DATA_TYPE first_clicked_button_flag;
	void addButton(Button *button);
	void check();

public:
	void process();
	ALL_BUTTONS_FLAGS_DATA_TYPE getData();
	void reset();
	void Init();
	ButtonsManager();
	virtual ~ButtonsManager();
};

extern ButtonsManager buttons_manager;

#endif /* CLASSES_BUTTONSMANAGER_H_ */
