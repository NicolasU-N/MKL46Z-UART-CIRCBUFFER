/*
 * pit.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef PIT_H_
#define PIT_H_

#include "MKL46Z4.h"
#include <stdbool.h>
//#include <stdio.h>

void pit_init_ch0(uint32_t ldval);

void pit_init_ch1(uint32_t ldval);

bool get_flag_ch0();

bool get_flag_ch1();

void clear_flag_ch0();

void clear_flag_ch1();



void set_timeout_0( void );

void set_timeout_1( void );

void pit_enable( void );

void pit_disable( void );


#endif /* PIT_H_ */
