/*
 * pwm.h
 *
 *  Created on: 30/03/2022
 *      Author: Nicolas
 */

#ifndef PWM_H_
#define PWM_H_

#include "board.h"
#include <limits.h>

#define TPM_SC_CMOD_CLK (1u)
#define TPM_SC_PS_DIV16 (0x4u)
#define TPM_SC_CLK_DIV16 ((TPM_SC_CMOD_CLK << TPM_SC_CMOD_SHIFT) | \
                          TPM_SC_PS_DIV16)
#define SIM_SOPT2_TPMSRC_MCGPLLCLK (1u << SIM_SOPT2_TPMSRC_SHIFT)
#define SIM_SOPT2_TPM_MCGPLLCLK_DIV2 (SIM_SOPT2_TPMSRC_MCGPLLCLK | \
                                            SIM_SOPT2_PLLFLLSEL_MASK)

#define PTE31_MUX_TPM0_CH4_OUT (3u << PORT_PCR_MUX_SHIFT)
#define SET_PTE31_TPM0_CH4_OUT (PORT_PCR_ISF_MASK | \
                                PTE31_MUX_TPM0_CH4_OUT)

#define TPM_CONF_DEFAULT (0u)
#define TPM_CNT_INIT (0u)

#define PWMPIN (31) //KL46 Reference manual pg.173 tpm0_ch4

void initPWM(uint16_t period);
int setDuty(uint16_t duty);
unsigned short scalePWM(unsigned short period, unsigned short input);

#endif /* PWM_H_ */
