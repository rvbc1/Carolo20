/*
 * Average.cpp
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#include <Average.h>


void Average::Init(){
	average = 0;
	for(uint32_t i = 0; i < maxSampleNumber; i++) measurements[i] = 0;
}
void Average::Calculate(){
	float sum = 0.f;
	for(uint32_t i = 0; i < maxSampleNumber; i++) sum += measurements[i];
	average = sum / (float)maxSampleNumber;
}

void Average::AddSample(float val){
	measurements[counter] = val;
	if(++counter >= maxSampleNumber) counter = 0;
}

float Average::getAverage(){
	Calculate();
	return average;
}

Average::Average(uint32_t arraySize) : measurements(new float[arraySize]) {
	maxSampleNumber = arraySize;
	Init();

}

Average::~Average() {
	delete[] measurements;
	// TODO Auto-generated destructor stub
}

