/*
 * comunication_state_machine.h
 *
 *  Created on: 12 sie 2021
 *      Author: huber
 */

#ifndef INC_COMUNICATION_STATE_MACHINE_H_
#define INC_COMUNICATION_STATE_MACHINE_H_



enum CommState {CSM_NONE, CSM_TOFS, CSM_MOT, CSM_TOFS_AND_MOT, CSM_SLEEP, CSM_SHUTDOWN };

struct CommStateMachineDataType{
    enum CommState state;


};

void CommStateMachineInit(struct CommStateMachineDataType *CSM)
    {
    CSM->state = CSM_NONE;
    }

/**
 * @brief Temperature State Machine runtime decides which perypherials must be shutdown and which are free to be active.
 * More information about PollVector ANd how it work at MSM_Runtime in MinirysboardStateMachine.h
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
 */

void CommSM_Runtime(struct CommStateMachineDataType *CSM, uint16_t *CommShMem ,uint8_t *PollVector)
    {
    switch(CSM->state)
	{
    case CSM_NONE:
	*PollVector = *PollVector & 0xE7;//  mask is 1110 0111 which means mot and tofs are forbidden
	break;
    case CSM_TOFS:
	*PollVector = *PollVector & 0xEF;//  mask is 1110 1111 which means mot is forbidden
	break;
    case CSM_MOT:
	*PollVector = *PollVector & 0xF7;//  mask is 1111 0111 which means tofs are forbidden
	break;
    case CSM_TOFS_AND_MOT:
	*PollVector = *PollVector & 0xFF;//  mask is 1111 1111 which means everything is permited
	break;
    case CSM_SLEEP:
	*PollVector = *PollVector & 0xE3;//  mask is 1110 0011 which means 5V mot, tos are forbidden
	break;
    case CSM_SHUTDOWN:
	*PollVector = *PollVector & 0xE3;//  mask is 1110 0011 which means 5V mot, tos are forbidden
	break;
    default:
	break;

	}
    }

#endif /* INC_COMUNICATION_STATE_MACHINE_H_ */
