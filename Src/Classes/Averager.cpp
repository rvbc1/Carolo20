/*
 * Average.cpp
 *
 *  Created on: 10.02.2020
 *      Author: Igor
 */

#include <Averager.h>


uint64_t Averager::process_counter = 0;
uint64_t Averager::max_process_counter_value = 1;
uint16_t Averager::avrg_objs_counter = 0;
Averager* Averager::all_avrg_objs [] = {0};
const uint32_t Averager::task_dt = 1u;


uint64_t NWD(uint64_t a, uint64_t b)
{
	uint64_t pom;

  	while(b!=0)
	{
    	pom = b;
    	b = a%b;
    	a = pom;
  	}
    return a;
}

uint64_t NWW(uint64_t a, uint64_t b)
{
	return a/NWD(a, b)*b;
}

void Averager::process(){
	for(uint16_t i = 0; i < avrg_objs_counter; i++){
		if((process_counter % all_avrg_objs[i]->getSamplingRate()) == 0){
			all_avrg_objs[i]->addNewSample();
		}
	}
	process_counter++;
	if(process_counter >= max_process_counter_value)
		process_counter = 0;
	osDelay(task_dt);
}

Averager* Averager::createAverager(int32_t* sample_pointer, uint32_t samples_amount, uint16_t sampling_rate){
	Averager* averager = nullptr;
	if(avrg_objs_counter < MAX_AVERAGING_OBJECTS_AMOUNT){
		averager = new Averager(sample_pointer, samples_amount, sampling_rate);
		max_process_counter_value = NWW(max_process_counter_value, sampling_rate);
		all_avrg_objs[avrg_objs_counter] = averager;
		avrg_objs_counter++;
	}
	return averager;
}

void Averager::init(){
	average = 0.0;
	for(uint32_t i = 0; i < averager_array_size; i++) measurements[i] = 0;
}
void Averager::calculate(){
	float sum = 0.f;
	for(uint32_t i = 0; i < averager_array_size; i++) sum += measurements[i];
	average = sum / (float)averager_array_size;
}

uint16_t Averager::getSamplingRate(){
	return sampling_rate;
}

void Averager::addNewSample(){
	measurements[counter] = *sample_pointer;
	if(++counter >= averager_array_size) counter = 0;
}

void Averager::addSample(float val){
	measurements[counter] = val;
	if(++counter >= averager_array_size) counter = 0;
}

float Averager::getAverage(){
	calculate();
	return average;
}

Averager::Averager(int32_t* sample_pointer, uint32_t samples_amount, uint16_t sampling_rate) : measurements(new float[samples_amount]) {
	averager_array_size = samples_amount;
	this->sample_pointer = sample_pointer;
	this->sampling_rate = sampling_rate;
	init();

}

Averager::~Averager() {
	delete[] measurements;
	// TODO Auto-generated destructor stub
}

