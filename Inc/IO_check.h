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

#include <stdio.h>
#include <string.h>



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
/**
 * this func is ment to run in 1s task.
 */
 uint8_t TEST_IO_RTOS(uint8_t test_command){

	static uint8_t state = 0;

	switch(state){
	case 0:
	    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
	    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
	    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);

		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);




		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
		HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 1);
		state = 1;
		break;
	case 1:
	    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
	    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
	    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
	    if (test_command == 1)
	    	{state++;}

		break;
	case 2:
		HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 0);

		char buffer [256];
		for (int i =0 ; i<256;++i)
		{
			buffer[i] = '\0';
		}

		for (int i = 0; i < 11 ; i++)
		{
			char tmp [32];
			sprintf(tmp,"ADC%d: %d\r\n" ,i , MachineStateData.AnalogInputs.ADCInput[i]);
			strcat(buffer,tmp);
		}


		HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

		state++;
		break;
	case 3:
	    HAL_GPIO_WritePin(ENABLE_RAIL_12V_GPIO_Port, ENABLE_RAIL_12V_Pin, 1);
	    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
	    if(test_command == 1 )
	    	state++;
	    else if(test_command == 2)
	    	state--;
	    break;
	case 4:
	    HAL_GPIO_WritePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin, 1);
	    if(test_command == 1 )
	    	state++;
	    else if(test_command == 2)
	    	state--;
	    break;
	case 5:
	    HAL_GPIO_WritePin(ENABLE_RAIL_5V_GPIO_Port, ENABLE_RAIL_5V_Pin, 1);
	    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
	    if(test_command == 1 )
	    	state++;
	    else if(test_command == 2)
	    	state--;
	    break;
	case 6:
		HAL_GPIO_WritePin(ENABLE_TOFS_GPIO_Port, ENABLE_TOFS_Pin, 1);
	    if(test_command == 1 )
	    	state++;
	    else if(test_command == 2)
	    	state--;
	    break;
	case 7:
	    HAL_GPIO_WritePin(ENABLE_STEPPER_MOTORS_GPIO_Port, ENABLE_STEPPER_MOTORS_Pin, 1);
	    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
	    if(test_command == 1 )
	    	state++;
	    else if(test_command == 2)
	    	state--;
	    break;
	case 8:
		HAL_GPIO_TogglePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin);
		HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
	    if(test_command == 2)
	    	state--;
	    break;
	default:
		break;
	}
return state;
}


#endif /* INC_IO_CHECK_H_ */
