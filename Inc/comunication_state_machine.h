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
    uint16_t sleep_timer;


};

void CommStateMachineInit(struct CommStateMachineDataType *CSM)
    {
    CSM->state = CSM_NONE;
    CSM->sleep_timer = 0;
    }

/**
 * @brief Comm State Machine runtime decides which perypherials must be shutdown and which are free to be active.
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

void CommSM_Runtime(struct CommStateMachineDataType *CSM, uint16_t *CommShMem ,uint8_t *PollVector, uint8_t power_button_state )
    {
    switch(CSM->state)
	{
    case CSM_NONE:
    	*PollVector = *PollVector & 0xE7;//  mask is 1110 0111 which means mot and tofs are forbidden
    	if(CommShMem[3] == 0x08)	//this sucks but will work for now.
    		CSM->state = CSM_SHUTDOWN;
    	else if(CommShMem[3] == 0x02)
    	{
    		CSM->state = CSM_SLEEP;
    		CSM->sleep_timer = CommShMem[4];
    		CommShMem[4] =0x00;
    	}
    	else if(CommShMem[3] == 0x04)
    		CSM->state = CSM_TOFS;
    	else if(CommShMem[3] == 0x10)
    		CSM->state = CSM_MOT;
    	else
    		goto CSM_Err; //goto land fill
    	break;
    case CSM_TOFS:
    	*PollVector = *PollVector & 0xEF;//  mask is 1110 1111 which means mot is forbidden
    	if(CommShMem[3] == 0x20)
    		CSM->state = CSM_NONE;
    	else if(CommShMem[3] == 0x40)
    		CSM->state = CSM_TOFS_AND_MOT;
    	else
    		goto CSM_Err;
    	break;
    case CSM_MOT:
    	*PollVector = *PollVector & 0xF7;//  mask is 1111 0111 which means tofs are forbidden
    	if(CommShMem[3] == 0x10)
			CSM->state = CSM_NONE;
		else if(CommShMem[3] == 0x80)
			CSM->state = CSM_TOFS_AND_MOT;
		else
			goto CSM_Err;
    	break;
    case CSM_TOFS_AND_MOT:
    	*PollVector = *PollVector & 0xFF;//  mask is 1111 1111 which means everything is permited
    	if(CommShMem[3] == 0x10)
    		CSM->state = CSM_NONE;
    	else if (CommShMem[3] == 0x100)
    		CSM->state = CSM_MOT;
    	else if (CommShMem[3] == 0x200)
    		CSM->state = CSM_TOFS;
    	else
    		goto CSM_Err;
    	break;
    case CSM_SLEEP:	//only time can take it from here this should be modified so mcu goas to sleep aswell
       	*PollVector = *PollVector & 0xE3;//  mask is 1110 0011 which means 5V mot, tos are forbidden
       	if(CSM->sleep_timer == 0)
       		CSM->state = CSM_NONE;
       	CSM->sleep_timer--;
    	break;
    case CSM_SHUTDOWN:	//only power button can take it from here. TODO make mcu go to sleep.
    	*PollVector = *PollVector & 0xE3;//  mask is 1110 0011 which means 5V mot, tos are forbidden
    	if(power_button_state == 1)
    		CSM->state = CSM_NONE;
    	break;
    default:
    	break;

	}

    //for future error handling
CSM_Err:
    //set own state to shm
    CommShMem[6] = CSM->state;
    CommShMem[3] = 0x00;


    }

#endif /* INC_COMUNICATION_STATE_MACHINE_H_ */
