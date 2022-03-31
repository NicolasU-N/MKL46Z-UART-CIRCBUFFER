/* Definiciones generales */
#ifndef VARIOS_H_
#define VARIOS_H_

/* Valores booleanos */
#define SI	1
#define NO	0

/* TIMERS */
#define NUM_PER		1 // 160Hz | 320Hz
#define NUM_TO		4
#define NUM_PWMS	1

#define N_PER_NORMAL		0
#define N_PER_LOW_INTENSITY	1

#define N_TO_DISPLAY_OFF			0
#define N_TO_LOW_INTENSITY			1
#define N_TO_NEW_DATA				2
#define N_TO_ACT_LOW_INTENSITY		3

#define N_PWM_PTE_31		0

/* XON - XOFF*/
#define XON		0x11//0x11
#define XOFF	0x13//0x13

/* BUFFER SIZE */
#define BUFF_SIZE	64

#endif
