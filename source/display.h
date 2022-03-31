/*
 * display.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stdint.h"
#include "MKL46Z4.h"

#define SEG_A_PIN       2U
#define SEG_B_PIN       3U
#define SEG_C_PIN       6U
#define SEG_D_PIN       16U
#define SEG_E_PIN       17U
#define SEG_F_PIN       18U
#define SEG_G_PIN       19U

#define COM_PIN			31U

//get k bits from p position
#define GET_LSB(number, k, p) (((1 << k) - 1) & (number >> (p - 1)))

void lcdInit();

void lcdScan(char idx);

void lcdOff();

#endif /* DISPLAY_H_ */
