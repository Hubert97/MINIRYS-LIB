/*
 * IO_check.h
 *
 *  Created on: Jul 15, 2021
 *      Author: huber
 *
 *      Do not use in normal setting this code is used to debug board - checks every voltage line.
 */

#ifndef INC_IO_CHECK_H_
#define INC_IO_CHECK_H_
#include "main.h"


void TEST_IO(){



    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 1);
    HAL_Delay(1000);

    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);



    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}
    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 0);


    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}

    HAL_GPIO_WritePin(ENABLE_RAIL_12V_GPIO_Port, ENABLE_RAIL_12V_Pin, 1);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);

    HAL_Delay(1000);
    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}


    HAL_GPIO_WritePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin, 1);

    HAL_Delay(1000);
    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}


    HAL_GPIO_WritePin(ENABLE_RAIL_5V_GPIO_Port, ENABLE_RAIL_5V_Pin, 1);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
    HAL_Delay(1000);
    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}

    HAL_GPIO_WritePin(ENABLE_TOFS_GPIO_Port, ENABLE_TOFS_Pin, 1);

    HAL_Delay(1000);
    while(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
	{
	HAL_Delay(10);
	}

    HAL_GPIO_WritePin(ENABLE_STEPPER_MOTORS_GPIO_Port, ENABLE_STEPPER_MOTORS_Pin, 1);
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);

    HAL_Delay(1000);
    while(1)
	{
	HAL_GPIO_TogglePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin);
	HAL_Delay(100);
	}




}
void Analog_Only(){



    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 1);
    HAL_Delay(1000);

    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 0);







}


#endif /* INC_IO_CHECK_H_ */
