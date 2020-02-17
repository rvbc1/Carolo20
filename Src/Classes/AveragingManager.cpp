/*
 * AveragingManager.cpp
 *
 *  Created on: 11.02.2020
 *      Author: rvbc-
 */

#include <AveragingManager.h>

void AveragingManager::init(){
	avrg_objs_counter = 0;
}

void AveragingManager::process(){
	for(uint16_t i = 0; i < avrg_objs_counter; i++){
		if((process_counter % all_avrg_objs[i]->getSamplingRate()) == 0){
			all_avrg_objs[i]->addNewSample();
		}
	}
}

Averager* AveragingManager::createAverager(uint16_t data, uint16_t sampling_rate){
	Averager* averager = NULL;
	if(avrg_objs_counter < MAX_AVERAGING_OBJECTS_AMOUNT){
		//averager = new Averager(12,sampling_rate);
		all_avrg_objs[avrg_objs_counter] = averager;
		avrg_objs_counter++;
	}
	return averager;
}

AveragingManager::AveragingManager() {

	// TODO Auto-generated constructor stub
}

AveragingManager::~AveragingManager() {
	// TODO Auto-generated destructor stub
}

