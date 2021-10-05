/*
 * ModbusConfig.h
 *
 *  Created on: 24 sie 2021
 *      Author: huber
 */

#ifndef INC_MODBUSCONFIG_H_
#define INC_MODBUSCONFIG_H_





/* Uncomment the following line to enable support for Modbus RTU over USB CDC profile. Only tested for BluePill f103 board. */
//#define ENABLE_USB_CDC 1

/* Uncomment the following line to enable support for Modbus TCP. Only tested for Nucleo144-F429ZI. */
//#define ENABLE_TCP 1


#define T35  5              // Timer T35 period (in ticks) for end of frame detection.
#define MAX_BUFFER  128	    // Maximum size for the communication buffer in bytes.
#define MAX_M_HANDLERS 2    //Maximum number of modbus handlers that can work concurrently
#define MAX_TELEGRAMS 2     //Max number of Telegrams in master queue




#endif /* INC_MODBUSCONFIG_H_ */
