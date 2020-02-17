/*
 * Average.h
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#ifndef CLASSES_AVERAGER_H_
#define CLASSES_AVERAGER_H_

#include "stdint.h"
#include "AllTasks.h"

#define MAX_AVERAGING_OBJECTS_AMOUNT 10


class Averager {
public:
	uint16_t getSamplingRate();
	void addNewSample();
	void addSample(float val);
	float getAverage();


	static Averager* createAverager(int32_t* sample_pointer, uint32_t samples_amount, uint16_t sampling_rate);
	static void process();
	virtual ~Averager();

private:
	uint32_t averager_array_size;
	float *measurements;
	uint32_t counter = 0;
	float average = 0.f;
	int32_t* sample_pointer;
	uint16_t sampling_rate;

	Averager(int32_t* sample_pointer, uint32_t samples_amount, uint16_t sampling_rate);

	static Averager* all_avrg_objs [MAX_AVERAGING_OBJECTS_AMOUNT];
	static uint64_t process_counter;
	static uint64_t max_process_counter_value;
	static uint16_t avrg_objs_counter;
	static const uint32_t task_dt;

	void calculate();
	void init();
};

#endif /* CLASSES_AVERAGER_H_ */
