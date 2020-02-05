/*
 * WatchDogs.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: Marek
 */

#include <WatchDogs.h>


void WatchDogs::init(){
	MX_IWDG_Init();
	MX_WWDG_Init();
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

