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

#include "Tiempo.h"
#include "pit.h"
#include "uart_ringBuffer.h"
#include "display.h"

//---------------BUFFER-----------------
ringBuferData_struct *pRingBufferRx;
char flag_xoff;	//1 xoff activated

char rxvalueMNS;
char ch;		// buffer character
//--------------------------------------

//---------------ESTADOS----------------
#define D_REPOSO		0
#define D_NORMAL		1
#define D_OFF_MODE		2
#define D_LOW_INT_MODE	3

char dis_state = D_REPOSO;

#define T_NORMAL_MODE	0
#define T_NEG_MODE		1

char tr_state = T_NORMAL_MODE;
//-------------------------------------

//---------------TIMER-----------------
Tm_Control c_tiempo;
static Tm_Periodo periodos[NUM_PER];
static Tm_Timeout timeouts[NUM_TO];

char flag_timeout_0 = NO; //display 2 seg
char flag_timeout_1 = NO; //display 5 seg
char flag_timeout_2 = NO; //display 10 seg "-"

static char atender_timer(char atienda);
//-------------------------------------

/*
 * @brief   Application entry point.
 */
int main(void) {

	clock_config();

	lcdInit();

	/* ============ UART0 ================ */
	uart0_init(SystemCoreClock, 2400); //115200

	/* ============ TIMER ================ */
	pit_init_ch0(0x124F7); //0xB71AFF -> 500ms || 0x124F7 -> 3.125ms

	Tm_Inicio(&c_tiempo, periodos, NUM_PER, timeouts, NUM_TO, &atender_timer);

	Tm_Inicie_periodo(&c_tiempo, N_PER_NORMAL, 2); //6.25 ms

	Tm_Inicie_timeout(&c_tiempo, N_TO_NEW_DATA, 3076); //10 SEG

	uart_send_byte(XON); //0x11 XON || 0x17 XON REALTERM

	myprintf("Hello World\r\n");

	while (1) {

		if (atender_timer(NO)) {
			Tm_Procese(&c_tiempo);
		}

		switch (dis_state) {

		case D_REPOSO:
			lcdOff();
			if (ringBuffer_getCount(pRingBufferRx) != 0) {
				dis_state = D_NORMAL;
			}
			break;

		case D_NORMAL:

			if (Tm_Hubo_periodo(&c_tiempo, N_PER_NORMAL)) { // 160Hz

				//-------------------------------------------------- OBTENER DATO DE BUFFER
				if (ringBuffer_getCount(pRingBufferRx) != 0) {

					ringBuffer_getData(pRingBufferRx, &ch);
					uart_send_string("C=");
					uart_send_byte(ch);
					myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));

					if (ringBuffer_isFull(pRingBufferRx)) {
						uart_send_byte(XOFF); // XOFF
						flag_xoff = SI;
					}
				}

				if (flag_xoff && ringBuffer_getCount(pRingBufferRx) == 0) {

					//-------------------------------- imprimir ultimo dato
					ringBuffer_getData(pRingBufferRx, &ch);
					ringBuffer_getData(pRingBufferRx, &ch);
					uart_send_string("C=");
					uart_send_byte(ch);
					myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));
					//--------------------------------

					uart_send_byte(XON); //0x11 XON || 0x17 XON REALTERM
					flag_xoff = NO;
				}
				//--------------------------------------------------------------------------

				//------------------------ ESTADOS
				switch (ch) {
				case 38: //&
					tr_state = T_NEG_MODE;
					break;
				case 37: //%
					uart_send_byte(XOFF); // XOFF
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_DISPLAY_OFF, 615); //2 SEG
					//TRX XOFF
					dis_state = D_OFF_MODE;
					break;
				case 36: //$ //NORMAL
					tr_state = T_NORMAL_MODE;
					break;
				case 35: //#
					dis_state = D_LOW_INT_MODE;
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_LOW_INTENSITY, 1538); //5 SEG
					Tm_Inicie_periodo(&c_tiempo, N_PER_LOW_INTENSITY, 1); //3.25 ms
					break;
				}

				switch (tr_state) {
				case T_NORMAL_MODE:
					rxvalueMNS = GET_LSB(ch, 4, 1);
					break;
				case T_NEG_MODE:
					rxvalueMNS = 15 - GET_LSB(ch, 4, 1);
					break;
				}
				//------------------------

				if (!flag_timeout_2 && ch !=38 && ch !=36 && ch !=35 && ch !=37 && ch !=13) {
					lcdScan(rxvalueMNS);
				}

				Tm_Baje_periodo(&c_tiempo, N_PER_NORMAL);
			}
			break;

		case D_OFF_MODE:
			lcdOff();
			if (flag_timeout_0
					!= Tm_Hubo_timeout(&c_tiempo, N_TO_DISPLAY_OFF)) {
				uart_send_string("CH EST\r\n");
				//cambia estado
				dis_state = D_NORMAL;
				//TRX XON
				uart_send_byte(XON); // XOFF
			}
			flag_timeout_0 = Tm_Hubo_timeout(&c_tiempo, N_TO_DISPLAY_OFF);
			break;

		case D_LOW_INT_MODE:

			if (Tm_Hubo_periodo(&c_tiempo, N_PER_LOW_INTENSITY)) { // 160Hz

				//-------------------------------------------------- OBTENER DATO DE BUFFER
				if (ringBuffer_getCount(pRingBufferRx) != 0) {

					ringBuffer_getData(pRingBufferRx, &ch);
					uart_send_string("C=");
					uart_send_byte(ch);
					myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));

					if (ringBuffer_isFull(pRingBufferRx)) {
						uart_send_byte(XOFF); // XOFF
						flag_xoff = SI;
					}
				}

				if (flag_xoff && ringBuffer_getCount(pRingBufferRx) == 0) {

					//-------------------------------- imprimir ultimo dato
					ringBuffer_getData(pRingBufferRx, &ch);
					ringBuffer_getData(pRingBufferRx, &ch);
					uart_send_string("C=");
					uart_send_byte(ch);
					myprintf(" CO=%d\r\n", ringBuffer_getCount(pRingBufferRx));
					//--------------------------------

					uart_send_byte(XON); //0x11 XON || 0x17 XON REALTERM
					flag_xoff = NO;
				}
				//--------------------------------------------------------------------------

				//------------------------ ESTADOS
				switch (ch) {
				case 38: //&
					tr_state = T_NEG_MODE;
					break;
				case 37: //%
					uart_send_byte(XOFF); // XOFF
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_DISPLAY_OFF, 615); //2 SEG
					//TRX XOFF
					dis_state = D_OFF_MODE;
					break;
				case 36: //$ //NORMAL
					tr_state = T_NORMAL_MODE;
					break;
				case 35: //#
					dis_state = D_LOW_INT_MODE;
					//iniciar time out
					Tm_Inicie_timeout(&c_tiempo, N_TO_LOW_INTENSITY, 1538); //5 SEG
					Tm_Inicie_periodo(&c_tiempo, N_PER_LOW_INTENSITY, 1); //3.25 ms
					break;
				}

				switch (tr_state) {
				case T_NORMAL_MODE:
					rxvalueMNS = GET_LSB(ch, 4, 1);
					break;
				case T_NEG_MODE:
					rxvalueMNS = 15 - GET_LSB(ch, 4, 1);
					break;
				}
				//------------------------
				if (!flag_timeout_2 && ch !=38 && ch !=36 && ch !=35 && ch !=37 && ch !=13) {
					lcdScan(rxvalueMNS);
				}

				Tm_Baje_periodo(&c_tiempo, N_PER_LOW_INTENSITY);
			}

			if (flag_timeout_1
					!= Tm_Hubo_timeout(&c_tiempo, N_TO_LOW_INTENSITY)) {
				uart_send_string("CH EST\r\n");
				//cambia estado
				dis_state = D_NORMAL;
			}
			flag_timeout_1 = Tm_Hubo_timeout(&c_tiempo, N_TO_LOW_INTENSITY);

			break;
		}

		if (flag_timeout_2 != Tm_Hubo_timeout(&c_tiempo, N_TO_NEW_DATA)) {
			uart_send_string("TO DISPLAY\r\n");
			lcdScan('-');
		}
		flag_timeout_2 = Tm_Hubo_timeout(&c_tiempo, N_TO_NEW_DATA);

	}
	return 0;
}

/* Rutina de verificación y atención del HW del timer */
static char atender_timer(char atienda) {
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		if (atienda)
			/* Clear interrupt flag.*/
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		return SI;
	} else {
		return NO;
	}
}
