/*
 * fan_regulator.h
 *
 *  Created on: 12 sie 2021
 *      Author: huber
 */

#ifndef INC_FAN_REGULATOR_H_
#define INC_FAN_REGULATOR_H_

struct FanRegulatorDataType{
    uint8_t state;


};

/**
 * @brief Temperature State Machine runtime decides which perypherials must be shutdown and which are free to be active.
 * More information about PollVector ANd how it work at MSM_Runtime in MinirysboardStateMachine.h
 *
 */

void FR_Runtime(struct FanRegulatorDataType *TSM, uint8_t *PollVector)
    {



    }

#endif /* INC_FAN_REGULATOR_H_ */
