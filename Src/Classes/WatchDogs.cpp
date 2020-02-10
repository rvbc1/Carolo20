/*
 * WatchDogs.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: Marek
 */

#include <WatchDogs.h>


void WatchDogs::init(){
	while(HAL_GetTick() < WatchDogs::start_wdg_after){
		osDelay(1);
	}

	MX_IWDG_Init();
	MX_WWDG_Init();
	osDelay(task_dt);
}
void WatchDogs::process(){
	HAL_WWDG_Refresh(&hwwdg);
	HAL_IWDG_Refresh(&hiwdg);

	osDelay(task_dt);
}

WatchDogs::WatchDogs() {
	// TODO Auto-generated constructor stub

}

WatchDogs::~WatchDogs() {
	// TODO Auto-generated destructor stub
}

