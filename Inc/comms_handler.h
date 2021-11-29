/*
 * comunication_state_machine.h
 *
 *  Created on: 12 sie 2021
 *      Author: huber
 */

#ifndef INC_COMMS_HANDLER_H
#define INC_COMMS_HANDLER_H
#include "main.h"

enum COM_Command {COM_WAITING = 0, COM_READ, COM_WRITE, COM_PING };

struct COM_timeout_timer{
	uint16_t ticks;
	uint8_t is_triggered;
};


union COM_Data{
	uint8_t buf8[2];
	uint16_t buf16;
};



struct COM_State{
    enum COM_Command state; //internal state
    uint8_t data_counter; // how many bits we have recieved
    struct COM_timeout_timer Timer;	//timout timer - if it triggers we resset the command
	uint8_t which; //which register we read / write to
	union COM_Data data; //16 bit data
	uint8_t response_buf [4];
};


void COM_State_Init(struct COM_State* com_state)
{
	com_state->state = COM_WAITING;
	com_state->data_counter = 0;
	com_state->Timer.ticks = 0;
	com_state->Timer.is_triggered = 0;
}


void COM_RX_Handler(struct COM_State* com_state,uint8_t data)
{
	com_state->data_counter ++;

	if(com_state->Timer.is_triggered)
	{
		com_state->Timer.is_triggered = 0;
		goto COM_Cleanup;
	}

	switch (com_state->state)
	{
	case COM_WAITING:
		switch(data)
		{
		case 0x01:
			com_state->state = COM_READ;
			break;
		case 0x02:
			com_state->state = COM_WRITE;
			break;
		case 0x03:
			//answer with 0x03 - PING
			com_state->response_buf[0] = 0x03;
			HAL_UART_Transmit_IT(&huart1, com_state->response_buf, 1);
			goto COM_Cleanup;
			break;
		default:
			goto COM_Error;
			break;
		}
		break;
	case COM_READ:
		switch (com_state->data_counter){
		case 2:
			com_state->which = data;
			break;
		case 3:
			if(data == COM_READ )
			{
				// execute command	//respond with data
				com_state->data.buf16 = ModbusDATA[com_state->which];
				com_state->response_buf [0] = COM_READ;
				com_state->response_buf [1] = com_state->data.buf8[0];
				com_state->response_buf [2] = com_state->data.buf8[1];
				com_state->response_buf [3] = COM_READ;
				HAL_UART_Transmit_IT(&huart1, com_state->response_buf, 4);

				goto COM_Cleanup;
			}
			else
			{
				goto COM_Error;
			}
			break;
		default:
			goto COM_Error;
			break;

		}
		break;
	case COM_WRITE:
		switch (com_state->data_counter){
		case 2:
			com_state->which = data;
			break;
		case 3:
			com_state->data.buf8[0] = data;
			break;
		case 4:
			com_state->data.buf8[1] = data;
			break;
		case 5:
			if(data == COM_WRITE)
			{
				ModbusDATA[com_state->which] = com_state->data.buf16;// execute command
				com_state->response_buf[0] = 0x02;	//respond with 0x02
				HAL_UART_Transmit_IT(&huart1, com_state->response_buf, 1);
				goto COM_Cleanup;
			}
			else
			{
				goto COM_Error;
			}
			break;
		default:
			goto COM_Error;
			break;
		}
		break;
	default:
		goto COM_Error;//error
		break;
	}
	return;

COM_Error:
	//SEND ERROR;
	com_state->response_buf[0] = 0x04;
	HAL_UART_Transmit_IT(&huart1, com_state->response_buf, 1);
COM_Cleanup:
	com_state->data_counter = 0;
	com_state->state = COM_WAITING;
	return;
}









#endif /* INC_COMMS_HANDLER_H */

