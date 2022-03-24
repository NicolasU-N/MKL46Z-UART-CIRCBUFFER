/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL46Z4_UART_LCD_BUFF.c
 * @brief   Application entry point.
 */
#include "board.h"

#include "uart_ringBuffer.h"
#include "display.h"
#include "pit.h"

#define NORMAL_MODE		0
#define NEG_MODE		1
#define OFF_MODE		2
#define DEC_INT_MODE	3

char state = NORMAL_MODE;

char rxvalueMNS;

char ch; 			// buffer character
bool dato_recibido = false;

/*
 * @brief   Application entry point.
 */
int main(void) {
	clock_config();

	/* ========== Init Display ========== */
	lcdInit();

	/* ============ UART0 ================ */
	uart0_init(SystemCoreClock, 2400); //115200

	myprintf("Hello World!\r\n");

	pit_init_ch0(0x249EF); // 0x249EF 6.25ms // 1B6BF -> 4.68 ms // 0xB71AFF -> 500ms

	while (1) {
		//ch = uart_receive_byte();
		//uart_send_byte(ch);

		//uart_receive_byte();

		if (get_flag_ch0()) { //160Hz
			dato_recibido = uart0_ringBuffer_getData(&ch, sizeof(ch));

			switch (ch) {
			case 38: //&
				state = NEG_MODE;
				break;
			case 37: //%

				break;
			case 36: //$ //NORMAL
				state = NORMAL_MODE;
				break;
			case 35: //#

				break;
			}

			switch (state) {
			case NORMAL_MODE:
				rxvalueMNS = GET_LSB(ch, 4, 1);
				break;
			case NEG_MODE:
				rxvalueMNS = 15 - GET_LSB(ch, 4, 1);
				break;
			}

			if (dato_recibido) {
				myprintf("RX_DATA -> %d\r\n", ch);
				lcdScan(rxvalueMNS);
			}
		}

	}

	return 0;
}
