/*
 * Average.h
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#ifndef CLASSES_AVERAGE_H_
#define CLASSES_AVERAGE_H_

#include "stdint.h"


class Average {
private:
	uint32_t maxSampleNumber;
	float *measurements;
	uint32_t counter = 0;
	float average = 0.f;

	void Calculate();
	void Init();

public:

	void AddSample(float val);
	float getAverage();
	Average(uint32_t arraySize);
	virtual ~Average();
};

#endif /* CLASSES_AVERAGE_H_ */
