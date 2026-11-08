/*
 * hub75e.h
 *
 *  Created on: Jul 17, 2026
 *      Author: sonam
 */

#ifndef INC_HUB75E_H_
#define INC_HUB75E_H_

#include "main.h"
#include "font5x7.h"
#include "timer.h"

void HUB75_TestRed(void);
void HUB75_TestGreen(void);
void HUB75_TestBlue(void);

void HUB75_Init(void);
void HUB75_fill_screen(const uint8_t red, const uint8_t green, const uint8_t blue);
void HUB75_ClearScreen(void);
void HUB75_SetPixel(const uint8_t x, const uint8_t y);
void HUB75_ClearPixel(const uint8_t x, const uint8_t y);
void HUB75_Display(void);
void HUB75_SwapBuffers(void);

void bouncing_ball(void);
void rectangle(void);
void snake(void);
void bresenhams_algorithm(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void circle_algorithm(uint8_t cx, uint8_t yc, uint8_t r);
void display_letter(const uint8_t* letter, const uint8_t x, const uint8_t y);

//void font5x7_DrawChar(char character,
//                      uint8_t start_row,
//                      uint8_t start_column);

//void font5x7_DrawString(const char *text,
//                        uint8_t start_row,
//                        uint8_t start_col);

#endif /* INC_HUB75E_H_ */
