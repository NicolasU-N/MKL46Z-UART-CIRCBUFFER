/*
 * pwm.c
 *
 *  Created on: 30/03/2022
 *      Author: Nicolas
 */
#include "pwm.h"

/**
 * @brief Initializes PWM on builtin blue LED / Pin D1
 * @param period Sets the period of the PWM signal. 1000 = 48kHz. 48000 = 500Hz
 */
void initPWM(uint16_t period) {
	// Enable clock to TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	/**Enable clock to port E*/
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// PWMPIN FTM0_CH1, MUX Alt 4
	PORTE->PCR[PWMPIN] = SET_PTE31_TPM0_CH4_OUT;

	//Configure TPM
	/* Set TPM clock source */
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPM_MCGPLLCLK_DIV2;

	// Continue operation in debug mode
	TPM0->CONF = TPM_CONF_DEFAULT;

	// Load the counter and mod
	TPM0->CNT = TPM_CNT_INIT;
	TPM0->MOD = 60000u;

	// Set channel 1 to edge-alligned high-true PWM
	TPM0->CONTROLS[4].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[4].CnV = 0;
	// Start TPM
	TPM0->SC = TPM_SC_CLK_DIV16;
}
/**
 * @param duty Dutycycle of the PWM signal. 0 - <period of PWM>
 * @return 1 if duty is greater than 2^16-1. Else 0
 */
int setDuty(uint16_t duty) {
	if (duty > 65535) {
		return 1;
	}
	TPM0->CONTROLS[4].CnV = duty;
	return 0;
}

/**
 * @brief Used to scale the input value to PWM according to the period being used
 * @param period Period of PWM
 * @param input Value to be scaled for use with PWM
 */
unsigned short scalePWM(unsigned short period, unsigned short input) {
	return (unsigned short) ((double) period
			* ((double) input / (double) USHRT_MAX));
}
