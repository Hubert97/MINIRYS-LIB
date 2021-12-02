/*
 * MinirysboardStateMachine.h
 *
 *  Created on: May 14, 2021
 *      Author: huber
 */

#ifndef INC_MINIRYSBOARD_STATE_MACHINE_UTILS_H_
#define INC_MINIRYSBOARD_STATE_MACHINE_UTILS_H_
#include "main.h"

#define analog_data uint16_t

#define MAX_CURRENT_INT 1234

#define MAX_12V_LINE_INT 1234
#define MIN_12V_LINE_INT 1234
#define MIN_5V_INT 1234
#define MAX_5V_INT 1234



union AnalogInputsData {
    struct _data{ // either this
        analog_data VolateProbe20V;
        analog_data VoltageProbe5V;

        analog_data ChasisTmp0;
        analog_data ChasisTmp1;
        analog_data BoardTmp0;
        analog_data BoardTmp1;

    	analog_data Cell1Voltage0;
    	analog_data Cell1Voltage1;
    	analog_data Cell1Voltage2;

        analog_data BatCurrent;
        analog_data BoardTmp2;

    } data; // or this
    volatile analog_data ADCInput[11];
} ;




struct MSM_InputsDataType
    {
	volatile analog_data ADCInput[11];


    };

typedef struct MSM_OutputsDataType
    {
	uint8_t DiodePWM [3]; // 0-R 1-G 2-B
	uint8_t FanSpeed;	//set value from outside
	uint8_t FanPWM;		//Real speed sent to fan



    } OutputsDataType;

 enum MSMState {MSM_SHUTDOWN, MSM_INIT, MSM_RUN_STATE, MSM_SLLEP};



    /**
     * @brief main state machine it contain children state machine data, It gathers, inteprets and sets pins of MCU.
     *
     */

typedef struct MSM_StateDataType
    {
	union AnalogInputsData AnalogInputs;
	//struct MSM_InputsDataType AnalogInputs;
	uint16_t FanSpeedRPM;
	uint8_t BoardConfiguration;
	enum MSMState state;
	struct MSM_OutputsDataType OutputData;

	// Voting State machies
	//todo


    } StateData;



    /**
      * @brief  Blindly copies data from one container to different - something like memcpy. Data type is analog_data
      * @note   This function will not check if its out of bounds caution required
      *
      *
      * @param Dest Destination Buffer address.
      * @param NoOfBytes Number of data to be transferred from ADC peripheral to memory
      *
      *
      */
void MSM_Cppy_Analog_Data(const union AnalogInputsData * Inputs)
    {

	/*
	 *
	 *
	 * This is because registers were set for diffrent
	 *  processor and i dont want to constrtrain this lib
	 *  to only one cpu so this inefficiency is neccecery
	 *  Too bad - somethin somethin chip shortage something
	 *
	 *   Template
	 *
		0-2 Cell1Voltage[3];
		3-4 ChasisTmp[2];
		5-7 BoardTemp[3];
		8 RPILineVoltage;
		9 VBatLineVoltage;
		10 BatCurrentVoltage;

	*/
	ModbusDATA[11] = Inputs->data.Cell1Voltage0;
	ModbusDATA[12] = Inputs->data.Cell1Voltage1;
	ModbusDATA[13] = Inputs->data.Cell1Voltage2;
	ModbusDATA[14] = Inputs->data.ChasisTmp0;
	ModbusDATA[15] = Inputs->data.ChasisTmp1;
	ModbusDATA[16] = Inputs->data.BoardTmp0;
	ModbusDATA[17] = Inputs->data.BoardTmp1;
	ModbusDATA[18] = Inputs->data.BoardTmp2;
	ModbusDATA[19] = Inputs->data.VoltageProbe5V;
	ModbusDATA[20] = Inputs->data.VolateProbe20V;
	ModbusDATA[21] = Inputs->data.BatCurrent;




    }
/**
 * @brief This function chcecks if analog sensors are working properly. they may be absent or shorted to gnd.
 */

int MSM_CheckAnalogSensors(struct MSM_StateDataType * Robot_State)
    {
    //todo
    //return 0 ; // nalog data is not valid
    return 1; //analog data is valid
    }

/**
 * @brief non blocking way to shutdown system - use in emegencies
 */
void MSM_EmergencyShutdown(struct MSM_StateDataType  * Robot_State)
    {
    HAL_GPIO_WritePin(ENABLE_RAIL_12V_GPIO_Port, ENABLE_RAIL_12V_Pin, 0);
    Robot_State->OutputData.FanPWM=0; //set fan speed to max
    HAL_GPIO_WritePin(PWM_FAN_GPIO_Port, PWM_FAN_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_RAIL_5V_GPIO_Port, ENABLE_RAIL_5V_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_TOFS_GPIO_Port, ENABLE_TOFS_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_STEPPER_MOTORS_GPIO_Port, ENABLE_STEPPER_MOTORS_Pin, 0);
    }

/**
 * @brief blocking function - disables sensors one by one and waits for robot to cool down.
 */
void MSM_Normal_Shutdown(struct MSM_StateDataType  * Robot_State)
    {
    HAL_GPIO_WritePin(ENABLE_TOFS_GPIO_Port, ENABLE_TOFS_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_STEPPER_MOTORS_GPIO_Port, ENABLE_STEPPER_MOTORS_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_RAIL_5V_GPIO_Port, ENABLE_RAIL_5V_Pin, 0);
    while( 0 )	//TODO check all items
	{
	Robot_State->OutputData.FanPWM=255; //set fan speed to max
	}
    Robot_State->OutputData.FanPWM=0; //set fan speed to max
    HAL_GPIO_WritePin(ENABLE_RAIL_12V_GPIO_Port, ENABLE_RAIL_12V_Pin, 0);
    }

/**
 * @brief determins board integrity and sets board_configuration value which means in what mode board is capable of runing
 * * 0 - board error - all of.
 * * 1 - board on external 5V - uart and analog sensor data.
 * * 2 - on leaah/battery - No Fan detected.
 * * 3 - on leash/battery - Fan detected.
 *
 * Important - this function should be run before OS.
 * @param[1] Robot_State
 *
 *
 */

void MSM_PreflightCheck(struct MSM_StateDataType  * Robot_State)
    {
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 1);
    HAL_Delay(1000);

    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);




    HAL_GPIO_WritePin(ENABLE_SENSORS_GPIO_Port, ENABLE_SENSORS_Pin, 0);
    HAL_Delay(10);	//wait for capacitors to charge up


 /*   if(!MSM_CheckAnalogSensors(Robot_State))	//board error
	{
	Robot_State->BoardConfiguration = 0;
	return;
	}
*/

    //Check if harness or batteries are present
    if(1)//todo check 12V line (Batt+)
	{
	//todo calibrate ADC and save data to flash.
	Robot_State->BoardConfiguration = 1;
	return;
	}
    //if passed than we know we are on harness or

    //enable 12V line
    HAL_GPIO_WritePin(ENABLE_RAIL_12V_GPIO_Port, ENABLE_RAIL_12V_Pin, 1);
    HAL_Delay(1);	//wait for capacitors to charge up
    //if current ok than done
    if(Robot_State->AnalogInputs.ADCInput[10] < MAX_CURRENT_INT)
	{
	    MSM_EmergencyShutdown(Robot_State);
	    Robot_State->BoardConfiguration = 0;

	    return;
	}

    Robot_State->BoardConfiguration = 2;
    return;
    }


#endif /* INC_MINIRYSBOARD_STATE_MACHINE_UTILS_H_ */
