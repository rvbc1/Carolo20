/*
 * WatchDogs.h
 *
 *  Created on: Jan 12, 2020
 *      Author: Marek
 */

#ifndef CLASSES_WATCHDOGS_H_
#define CLASSES_WATCHDOGS_H_

#include "iwdg.h"
#include "wwdg.h"
#include "cmsis_os.h"


class WatchDogs {
public:
	static void init();
	static void process();
	WatchDogs();
	virtual ~WatchDogs();
private:
	static const uint32_t task_dt = 1u;
};

#endif /* CLASSES_WATCHDOGS_H_ */
