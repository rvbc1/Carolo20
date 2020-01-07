/*
 * OLED.h
 *
 *  Created on: Jan 13, 2019
 *      Author: Marek
 */

#ifndef CLASSES_OLED_H_
#define CLASSES_OLED_H_

#define OLED_BUFFER_SIZE 8192


#include <AllTasks.h>
#include "alphabet.h"

class OLED {
private:
	uint8_t buffer [OLED_BUFFER_SIZE];

	void send_REG(uint8_t cmd);
	void send_DATA(uint8_t data);
	void OLED_Init_Registers();
	void OLED_reset();
	void reset_buffer();
	void update_all_screen();
public:
	void Init();
	void setImage(unsigned int imageId);
	void process();
	OLED();
	virtual ~OLED();
};

extern OLED oled;
extern int counter;
#endif /* CLASSES_OLED_H_ */
