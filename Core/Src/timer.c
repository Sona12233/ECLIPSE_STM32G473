/*
 * timer.c
 *
 *  Created on: Aug 4, 2026
 *      Author: sonam
 */

#include "timer.h"

volatile uint8_t timer3s_flag = 0;
volatile uint8_t timer1s_flag = 0;
volatile uint8_t timer500ms_flag = 0;
volatile uint8_t timer100ms_flag = 0;
volatile uint8_t timer50ms_flag = 0;

volatile uint8_t timer_counter = 0;

// for scrolling function
volatile uint8_t timer3s_scrolling_flag = 0;
volatile uint8_t timer3s_scrolling_counter = 0;

extern TIM_HandleTypeDef htim1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim1) {
    if (htim1->Instance == TIM1) {
    	++timer_counter; ++timer3s_scrolling_counter;
    	if (timer_counter == 240) timer_counter = 0;

    	timer50ms_flag = 1;

    	if (timer_counter % 2 == 0) { // counting 100ms
    		timer100ms_flag = 1;
    	}

    	if (timer_counter % 10 == 0) { // 500ms
    		timer500ms_flag = 1;
    	}

    	if (timer_counter % 20 == 0) { // 1 second
    		timer1s_flag = 1;
    	}

    	if (timer_counter % 60 == 0) { // 3 second
    		timer3s_flag = 1;
    	}

//    	if (timer3s_scrolling_enable) {
//			if (timer_counter == 60) { // 3 seconds
//				timer3s_flag = 1;
//				timer_counter = 0;
//			}
//    	} else timer_counter = 0;

//    	if ((uint32_t)(timer_counter - timer3s_start_counter) >= 600
//    			&& timer3s_scrolling_enable) {
//			timer3s_flag = 1;
//			timer3s_scrolling_flag = 0;
////			timer_counter = 0;
//    	}

    	if (timer3s_scrolling_counter >= 60) {
    		timer3s_scrolling_flag = 1;
    		timer3s_scrolling_counter = 0;
    	}

    }
}
