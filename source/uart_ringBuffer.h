/*
 * uart_ringBuffer.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef UART_RING_BUFFER_H_
#define UART_RING_BUFFER_H_

#include <string.h>
#include "stdint.h"
#include <stdarg.h>
#include <math.h>

#include "ringBuffer.h"
#include "KL46z.h"

extern ringBuferData_struct *pRingBufferRx;

#ifdef __cplusplus
extern "C" {
#endif

void clock_config();

void uart0_init(unsigned int uart0clk, unsigned int baud_rate);

void uart_send_byte(char data_input);

void uart_send_string(unsigned char *str_data);

char uart_receive_byte();

void UART0_IRQHandler();

/** @brief recibe datos por puerto serie accediendo al RB
 **
 ** @param[inout] pBuf buffer a donde guardar los datos
 ** @param[in] size tamaño del buffer
 ** @return cantidad de bytes recibidos
 **/
bool uart0_ringBuffer_getData(char *pBuf, int32_t size);

void myprintf(char*, ...);

#ifdef __cplusplus
}
#endif

#endif /* UART_RING_BUFFER_H_ */
