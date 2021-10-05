/*
 * temperature_state_machine.h
 *
 *  Created on: 12 sie 2021
 *      Author: huber
 */

#ifndef INC_TEMPERATURE_STATE_MACHINE_H_
#define INC_TEMPERATURE_STATE_MACHINE_H_
#include <minirysboard_state_machine_utils.h>

#define analog_data uint16_t
#define TEMP_60_DEG 1234
#define TEMP_50_DEG 1234
#define TEMP_70_DEG 1234

enum TState {TSM_MAX_FAN, TSM_RPI_CONTROL, TSM_MAX_FAN_VETO_VMOT, TSM_MAX_FAN_5V_VETO_VMOT_VETO };

struct TStateMachineDataType{
    enum TState state;


};

void TStateMachineInit(struct TStateMachineDataType *TSM)
    {
    TSM->state = TSM_MAX_FAN;
    }

/**
 * @brief Temperature State Machine runtime decides which peripherals must be shutdown and which are free to be active.
 * More information about PollVector ANd how it work at MSM_Runtime in MinirysboardStateMachine.h
 *

 * __7___6___5___4___3___2___1___0__
 * | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | - initial vector - some outputs are diabled due to machine state (i e no fan detected
 *
 * 7 - unused
 * 6 - FAN Enable
 * 5 - FAN Emergency mode enable
 * 4 - VMot Enable
 * 3 - TOF Enable
 * 2 - 5V Enable
 * 1 - BATT Enable
 * 0 - External sensors
 *
 * analog inputs format
 * 0-2 Cell1Voltage[3];
 * 3-4 ChasisTmp[2];
 * 5-7 BoardTemp[3];
 * 8 RPILineVoltage;
 * 9 VBatLineVoltage;
 * 10 BatCurrentVoltage;

 *
 */

void TSM_Runtime(struct TStateMachineDataType *TSM, analog_data * analog_inputs, uint8_t *PollVector)
    {
    switch(TSM->state)
	{
    case TSM_MAX_FAN:
	/*
	 * If temp low eouth and comms from rpi go to state TSM_RPI_COMMAND
	 */
	if(	analog_inputs[3] < TEMP_60_DEG &&
		analog_inputs[4] < TEMP_60_DEG &&
		analog_inputs[5] < TEMP_50_DEG &&
		analog_inputs[6] < TEMP_50_DEG &&
		analog_inputs[7] < TEMP_50_DEG &&
		1 //todo put check for comms fith rpi
	)
	*PollVector = *PollVector & 0xFF;//  mask is 1111 1111 which means all is permited and emergency fan is on
	break;
    case TSM_RPI_CONTROL:
	/*
	 * If any tem sensor is over threshoald then go to max FAN
	 */
	if(	analog_inputs[3] > TEMP_60_DEG ||
		analog_inputs[4] > TEMP_60_DEG ||
		analog_inputs[5] > TEMP_50_DEG ||
		analog_inputs[6] > TEMP_50_DEG ||
		analog_inputs[7] > TEMP_50_DEG ||
		0 //todo put check for comms fith rpi
	)
	*PollVector = *PollVector & 0xDF;//  mask is 1101 1111 which means all is permited and emergency fan is off
	break;
    case TSM_MAX_FAN_VETO_VMOT:
	break;
	*PollVector = *PollVector & 0xEF;//  mask is 1110 1111 which means emergency fan on, VMOT off
    case TSM_MAX_FAN_5V_VETO_VMOT_VETO:
	*PollVector = *PollVector & 0xEF;//  mask is 1110 0011 which means emergency fan on, VMOT off %V off and TOF off
	break;



	}


    }

#endif /* INC_TEMPERATURE_STATE_MACHINE_H_ */
