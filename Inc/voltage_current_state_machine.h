/*
 * voltage_current_state_machine.h
 *
 *  Created on: 12 sie 2021
 *      Author: huber
 */

#ifndef INC_VOLTAGE_CURRENT_STATE_MACHINE_H_
#define INC_VOLTAGE_CURRENT_STATE_MACHINE_H_
#define analog_data uint16_t


enum VCState {VCSM_ALL_OFF, VCSM_MAIN_12V_ON, VCSM_MAIN_12V_5V_ON, VCSM_MOT, VCSM_TOFS, VCSM_TOFS_MOT };

struct VCtateMachineDataType{
   enum VCState state;


};

void VCStateMachineInit(struct VCtateMachineDataType *VCSM)
    {
    VCSM->state = VCSM_ALL_OFF;
    }

/**
 * @brief Temperature State Machine runtime decides which perypherials must be shutdown and which are free to be active.
 *
 *
 * * More information about PollVector ANd how it work at MSM_Runtime in MinirysboardStateMachine.h
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
 */

void VCSM_Runtime(struct VCtateMachineDataType *VCSM, analog_data *analog_inputs,const uint8_t *PreviousPollVector , uint8_t *PollVector)
    {
    //static uint_fast8_t counter;
    switch(VCSM->state)
	{
    case VCSM_ALL_OFF:
	*PollVector = *PollVector & 0x81; // mask is 1000 0001 which means all non 3.3V efectors are forbidden
	break;
    case VCSM_MAIN_12V_ON:

	*PollVector = *PollVector & 0xE3; // mask is 1110 0011 which means only 12V line is perrmited
	break;
    case VCSM_MAIN_12V_5V_ON:

	*PollVector = *PollVector & 0xE7; // mask is 1110 0111 which means 5V and 12 V is permiteed
	break;
    case VCSM_MOT:

	*PollVector = *PollVector & 0xFE; // mask is 1111 0111 which means only tofs forbidden
	break;
    case VCSM_TOFS:

	*PollVector = *PollVector & 0xEF; // mask is 1110 1111 which means only mot is forbidden
	break;
    case VCSM_TOFS_MOT:

	*PollVector = *PollVector & 0xFF; // mask is 1111 1111 which means none is forbidden
	break;
    default:
	break;

	}

    }


#endif /* INC_VOLTAGE_CURRENT_STATE_MACHINE_H_ */
