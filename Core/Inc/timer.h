/*
 * timer.h
 *
 *  Created on: Aug 4, 2026
 *      Author: sonam
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>
#include "main.h"

extern volatile uint8_t timer3s_flag;
extern volatile uint8_t timer1s_flag;
extern volatile uint8_t timer500ms_flag;
extern volatile uint8_t timer100ms_flag;
extern volatile uint8_t timer50ms_flag;

// for scrolling function
extern volatile uint8_t timer3s_scrolling_flag;
extern volatile uint8_t timer3s_scrolling_counter;

void Timer_Reset3s_Scrolling(void);

void Timer_Start(void);
void Timer_Callback(TIM_HandleTypeDef *htim);

#endif /* INC_TIMER_H_ */
