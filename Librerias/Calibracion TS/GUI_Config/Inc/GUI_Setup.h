/*
 * GUI_Setup.h
 *
 *  Created on: Jan 7, 2022
 *      Author: Lenovo
 */

#ifndef INC_GUI_SETUP_H_
#define INC_GUI_SETUP_H_

#include "main.h"

#include "rtc.h"
#include "calibration.h"





extern TIM_HandleTypeDef TimHandle;

void BSP_Config(void);
void BSP_Pointer_Update(void);
void BSP_Background(void);

void TIM3_Config(void);



#endif /* INC_GUI_SETUP_H_ */
