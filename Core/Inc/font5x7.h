/*
 * font5x7.h
 *
 *  Created on: Oct 14, 2026
 *      Author: Sona
 */

#ifndef INC_FONT5X7_H_
#define INC_FONT5X7_H_

#include "hub75e.h"
#include <stdint.h>
#include <string.h>

#define FONT_HEIGHT 7
#define FONT_WIDTH  5

void font5x7_DrawChar(char character, uint8_t start_row, int8_t start_column);

void font5x7_DrawStringCentered(const char* text);

void font5x7_display_one_line(const char* text, const uint8_t size,
		const uint8_t start_row, uint8_t start_col);

void font5x7_display_two_line(const char* text, const uint8_t size);

void HUB75_ReplaceTexts(const char* text[], const uint8_t SIZE);

void HUB75_ScrollTexts(const char* text[], const uint8_t SIZE);

//void font5x7_display_three_line(const char* text, const uint8_t size);

#endif /* INC_FONT5X7_H_ */
