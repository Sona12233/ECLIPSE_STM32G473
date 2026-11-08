/*
 * hub75e.c
 *
 *  Created on: Jul 17, 2026
 *      Author: sonam
 */
#include "hub75e.h"
#include "timer.h"
#include "font5x7.h"

#include <stdlib.h>

static uint8_t framebuffer1[32][64];
static uint8_t framebuffer2[32][64]; // modifying functions

static uint8_t (*front_buffer)[64] = framebuffer1; //
static uint8_t (*back_buffer)[64] = framebuffer2; // modifying functions

static void HUB75_Clear(void)
{
    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B1_GPIO_Port, B1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B2_GPIO_Port, B2_Pin, GPIO_PIN_RESET);
}

void HUB75_TestRed(void)
{
    HUB75_Clear();

    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
}

void HUB75_TestGreen(void)
{
    HUB75_Clear();

    HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, GPIO_PIN_SET);
}

void HUB75_TestBlue(void)
{
    HUB75_Clear();

    HAL_GPIO_WritePin(B1_GPIO_Port, B1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(B2_GPIO_Port, B2_Pin, GPIO_PIN_SET);
}

static inline void CLK_Pulse(void)
{
    HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET);
}

static inline void LAT_Pulse(void)
{
    HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_RESET);
}

void HUB75_fill_screen(const uint8_t red, const uint8_t green, const uint8_t blue)
{
	for (uint8_t row = 0; row < 16; ++row) {

		// Disable display while loading data
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);

		// Select row
		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, (row & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, (row & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, (row & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, (row & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

		// Keep E low for a 64x32 panel
		HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);

		for (int i = 0; i < 64; ++i) {
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, red   ? GPIO_PIN_SET : GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, red   ? GPIO_PIN_SET : GPIO_PIN_RESET);

			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
			HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);

			HAL_GPIO_WritePin(B1_GPIO_Port, B1_Pin, blue  ? GPIO_PIN_SET : GPIO_PIN_RESET);
			HAL_GPIO_WritePin(B2_GPIO_Port, B2_Pin, blue  ? GPIO_PIN_SET : GPIO_PIN_RESET);

			CLK_Pulse();
		}
		LAT_Pulse();
		// Enable display
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
	}
    // Disable display after finishing all 16 addresses
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
}

void HUB75_ClearScreen(void) {
	for (int x = 0; x < 32; ++x) {
		for (int y = 0; y < 64; ++y) {
			back_buffer[x][y] = 0;
		}
	}
}

void HUB75_SetPixel(const uint8_t x, const uint8_t y) {
	if (x >= 32 || y >= 64) return;

	back_buffer[x][y] = 1;
}

void HUB75_ClearPixel(const uint8_t x, const uint8_t y) {
	if (x >= 32 || y >= 64) return;

	back_buffer[x][y] = 0;
}

void HUB75_SwapBuffers(void) {
    uint8_t (*temp)[64] = front_buffer;

    front_buffer = back_buffer;
    back_buffer = temp;
}

void HUB75_Display(void) {
	for (uint8_t row = 0; row < 16; ++row) {

		// Disable display while loading data
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);

		// Select row
		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, (row & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, (row & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, (row & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, (row & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

		// Keep E low for a 64x32 panel
		HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);

		for (int i = 0; i < 64; ++i) {

            uint8_t top_pixel = front_buffer[row][i];
            uint8_t bottom_pixel = front_buffer[row + 16][i];

			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, top_pixel    ? GPIO_PIN_SET : GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, bottom_pixel ? GPIO_PIN_SET : GPIO_PIN_RESET);

			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(B1_GPIO_Port, B1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(B2_GPIO_Port, B2_Pin, GPIO_PIN_RESET);

			CLK_Pulse();
		}
		LAT_Pulse();

		// Enable display
		HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
	}
    // Disable display after finishing all 16 addresses
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
}

//void bouncing_ball() {
//	uint8_t x = 0;
//	uint8_t y = 0;
//	uint8_t dx = 1;
//	uint8_t dy = 1;
//	while(1) {
//		HUB75_Display();
//		if (timer100ms_flag) {
//		  timer100ms_flag = 0;
//
//		  HUB75_ClearScreen();
//
//		  HUB75_SetPixel(x, y);
//
//		  if (x < 31 && dx) {
//			  ++x;
//		  }
//		  else if ((!dx || x >= 31) && x > 0) {
//			  --x;
//			  dx = 0;
//		  }
//		  else if (x <= 0 && !dx) {
//			  ++x;
//			  dx = 1;
//		  }
//
//
//		  if (y < 63 && dy) {
//			  ++y;
//		  }
//		  else if ((!dy || y >= 63) && y > 0) {
//			  --y;
//			  dy = 0;
//		  }
//		  else if (y <= 0 && !dy) {
//			  ++y;
//			  dy = 1;
//		  }
//		}
//	}
//}

void bouncing_ball() {
	uint8_t x = 0;
	uint8_t y = 0;
	int8_t dx = 1;
	int8_t dy = 1;
	while(1) {
		HUB75_Display();
		if (timer100ms_flag) {
		  timer100ms_flag = 0;

		  HUB75_ClearScreen();

		  HUB75_SetPixel(x, y);
		  x += dx; y += dy;

		  if (x >= 31 || x == 0) {
			  dx *= -1;
		  }

		  if (y >= 63 || y == 0) {
			  dy *= -1;
		  }

		}
	}
}

void rectangle() {
	uint8_t x = 0;
	uint8_t y = 0;
	int8_t dx = 1;
	int8_t dy = 1;

	while(1) {
		HUB75_Display();
		if (timer100ms_flag) {
			timer100ms_flag = 0;

			HUB75_ClearScreen();

			for (uint8_t row = 0; row <= x; ++row) {
				for (uint8_t column = 0; column <= y; ++column) {

					HUB75_SetPixel(row,column);
				}
			}
			if (x <= 31 || (dy < 0 && dx < 0)) x += dx;
			y += dy;


			if ((dy < 0 && y == 32) || x == 0) {
				dx = -dx;
			}
			if (y >= 63 || y == 0) {
				dy = -dy;
			}
		}
	}
}

void snake() {
	uint8_t y = 0;


	uint8_t snake[4] = {0, 1, 2, 3};
	for (int i = 0; i < 4; ++i) {
		HUB75_SetPixel(0, snake[i]);
	}
	while(1) {

		HUB75_Display();

		if (timer100ms_flag) {
			timer100ms_flag = 0;

			if (y >= 1) HUB75_ClearPixel(0, y - 1);
			HUB75_SetPixel(0, y + 3);

			++y;
//			++snake[i];
			if (y == 64) y = 0;

		}
	}
}

void bresenhams_algorithm(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	int8_t dx = abs(x2 - x1);
	int8_t dy = abs(y2 - y1);

	int8_t min = (dx < dy) ? dx : dy;
	int8_t max = (dx > dy) ? dx : dy;

	int8_t error = 0;

	uint8_t x = x1;
	uint8_t y = y1;


	while (1) {
		HUB75_SetPixel(x,y);

		if (x == x2 && y == y2) {
//			HUB75_SetPixel(x2,y2);
			break;
		}

		if (dx > dy) {
			if (x2 >= x1) ++x;
			         else --x;
		} else if (y2 >= y1) ++y;
					 else --y;

//		if (x == x2 || y == y2) {
//			HUB75_SetPixel(x2,y2);
//			break;
//		}

		error += min;

		if (error >= max) {
			if (dx > dy) {
				if  (y2 >= y1) ++y;
						  else --y;
			}
			else if (x2 >= x1) ++x;
	                      else --x;
			error -= max;
		}


	}

	while(1) HUB75_Display();
}

void circle_algorithm(uint8_t cx, uint8_t cy, uint8_t r) {
	uint8_t x = 0;
	uint8_t y = r;

	int8_t error = 1 - r;

	HUB75_ClearScreen();

	while(x <= y) {
		HUB75_SetPixel(cx + x, cy + y);
		HUB75_SetPixel(cx + x, cy - y);
		HUB75_SetPixel(cx - x, cy + y);
		HUB75_SetPixel(cx - x, cy - y);

		HUB75_SetPixel(cx + y, cy + x);
		HUB75_SetPixel(cx + y, cy - x);
		HUB75_SetPixel(cx - y, cy + x);
		HUB75_SetPixel(cx - y, cy - x);

		++x;

		if (error < 0) error += 2 * x + 1;
		else {
			--y;
			error += 2 * (x - y) + 1;
		}
	}
	while(1) HUB75_Display();

}

//void deisplay_letter(const uint8_t* letter, const uint8_t x, const uint8_t y) {
//
//	for (uint8_t row = 0; row < 7; ++row) {
//		for (uint8_t column = 0; column <= 4; ++column) {
//			if (H[row] >> (4 - column) & 1)
//			HUB75_SetPixel(row, column);
//		}
//	}
//	while(1) HUB75_Display();
//}

//void font5x7_DrawChar(char character,
//                      uint8_t start_row,
//                      uint8_t start_column) {
//
//	if (character < 'A' || character > 'Z') return;
//
//	uint8_t index = character - 'A';
//	//uint8_t temp[FONT_HEGIHT] = font[index][];
//	for (uint8_t row = 0; row < FONT_HEIGHT; ++row) {
//		for (uint8_t column = 0; column < FONT_WIDTH; ++column) {
//			if (font[index][row] >> (4 - column) & 1)
//			HUB75_SetPixel(row + start_row, column + start_column);
//		}
//	}
//}

//void font5x7_DrawString(const char *text,
//                        uint8_t start_row,
//                        uint8_t start_col) {
//
//}
