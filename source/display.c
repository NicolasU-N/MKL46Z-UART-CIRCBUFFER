/*
 * display.c
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#include "display.h"

//codes of symbols
//segments: g-f-e-d-c-b-a
const uint32_t scode[] = { //g -- a
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (0 << SEG_G_PIN), //0b0111111, //0     //0
		(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN)
				| (0 << SEG_G_PIN), //0b0000110, //1     //1
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (0 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (0 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1011011, //2     //2
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1001111, //3     //3
		(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1100110, //4     //4
		(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1101101, //5     //5
		(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1111101, //6     //6
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (0 << SEG_D_PIN) | (0 << SEG_E_PIN) | (0 << SEG_F_PIN)
				| (0 << SEG_G_PIN), //0b0000111, //7     //7
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1111111, //8     //8
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (0 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1101111, //9     //9
//0b00000000, //10    //space
		(1 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (0 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1110111, //10    //A
		(0 << SEG_A_PIN) | (0 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1111100, //11    //B
		(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (0 << SEG_G_PIN), //0b0111001, //12    //C
		(0 << SEG_A_PIN) | (1 << SEG_B_PIN) | (1 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (0 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1011110, //13    //D
		(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN)
				| (1 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1111001, //14    //E
		(1 << SEG_A_PIN) | (0 << SEG_B_PIN) | (0 << SEG_C_PIN)
				| (0 << SEG_D_PIN) | (1 << SEG_E_PIN) | (1 << SEG_F_PIN)
				| (1 << SEG_G_PIN), //0b1110001, //15    //F
		};

void lcdInit() {
	/* Initialize and enable LED */
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // Enable clock  to PORT E

	PORTE->PCR[SEG_A_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_B_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_C_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_D_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_E_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_F_PIN] |= PORT_PCR_MUX(1); // PORT E MUX
	PORTE->PCR[SEG_G_PIN] |= PORT_PCR_MUX(1); // PORT E MUX

	PTE->PDDR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN)
			| (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN)
			| (1u << SEG_G_PIN); // Outputs
	PTE->PSOR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN)
			| (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN)
			| (1u << SEG_G_PIN); //OFF leds
}

void lcdScan(char idx) {
	//turn off all digits
	PTE->PSOR |= (1u << SEG_A_PIN) | (1u << SEG_B_PIN) | (1u << SEG_C_PIN)
			| (1u << SEG_D_PIN) | (1u << SEG_E_PIN) | (1u << SEG_F_PIN)
			| (1u << SEG_G_PIN); //OFF led

	PTE->PCOR |= scode[idx];
}

