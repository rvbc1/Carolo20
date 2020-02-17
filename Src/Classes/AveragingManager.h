/*
 * AveragingManager.h
 *
 *  Created on: 11.02.2020
 *      Author: rvbc-
 */

#ifndef CLASSES_AVERAGINGMANAGER_H_
#define CLASSES_AVERAGINGMANAGER_H_

#include "main.h"
#include <Averager.h>

#define MAX_AVERAGING_OBJECTS_AMOUNT 10

class AveragingManager {
public:
	void init();
	void process();
	Averager* createAverager(uint16_t data, uint16_t sampling_rate);
	AveragingManager();
	virtual ~AveragingManager();
private:
	uint64_t process_counter = 0;
	uint16_t avrg_objs_counter = 0;
	Averager* all_avrg_objs [MAX_AVERAGING_OBJECTS_AMOUNT];
};

#endif /* CLASSES_AVERAGINGMANAGER_H_ */
