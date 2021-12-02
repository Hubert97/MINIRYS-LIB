/*
 * fan_state_machine.h
 *
 *  Created on: Jul 15, 2021
 *      Author: huber
 *
 *      Do not use in normal setting this code is used to debug board - checks every voltage line.
 */

#ifndef FAN_STATE_MACHINE_H_
#define FAN_STATE_MACHINE_H_

#include "main.h"

struct FAN_InterfaceS{
   uint8_t set_val;
   uint8_t PWM_counter;
   // todo count fan input impulses.


};


void FAN_Runtime(struct FAN_InterfaceS * fanS)
{
	fanS->PWM_counter++;
	if(fanS->PWM_counter < fanS->set_val)
	{
		HAL_GPIO_WritePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin, 1);
	}
	else
	{
		HAL_GPIO_WritePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin, 0);
	}
}


#endif /* FAN_STATE_MACHINE_H_ */

