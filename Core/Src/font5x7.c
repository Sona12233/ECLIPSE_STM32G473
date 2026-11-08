/*
 * font5x7.c
 *
 *  Created on: Oct 14, 2026
 *      Author: Sona
 */

#include "font5x7.h"

#define FONT_HEIGHT 7
#define FONT_WIDTH  5

static uint8_t keep_line(const char* text, uint8_t index, const uint8_t LAST_WORD, const uint8_t ANOTHER_WORD, const uint8_t SIZE);
static void drawStringAt(const char *text, int16_t start_col);


static const uint8_t font[][FONT_HEIGHT] =
{
    {
        // 0 : 'A'
        0b01110,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    },
    {
        // 1 : 'B'
        0b11110,
        0b10001,
        0b10001,
        0b11110,
        0b10001,
        0b10001,
        0b11110
    },
    {
        // 2 : 'C'
        0b01110,
        0b10001,
        0b10000,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    },
    {
        // 3 : 'D'
        0b11110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b11110
    },
    {
        // 4 : 'E'
        0b11111,
        0b10000,
        0b10000,
        0b11110,
        0b10000,
        0b10000,
        0b11111
    },
    {
        // 5 : 'F'
        0b11111,
        0b10000,
        0b10000,
        0b11110,
        0b10000,
        0b10000,
        0b10000
    },
    {
        // 6 : 'G'
        0b01110,
        0b10001,
        0b10000,
        0b10111,
        0b10001,
        0b10001,
        0b01110
    },
    {
        // 7 : 'H'
        0b10001,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    },
    {
        // 8 : 'I'
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b11111
    },
    {
        // 9 : 'J'
        0b00111,
        0b00010,
        0b00010,
        0b00010,
        0b10010,
        0b10010,
        0b01100
    },
    {
        // 10 : 'K'
        0b10001,
        0b10010,
        0b10100,
        0b11000,
        0b10100,
        0b10010,
        0b10001
    },
    {
        // 11 : 'L'
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b11111
    },
    {
        // 12 : 'M'
        0b10001,
        0b11011,
        0b10101,
        0b10101,
        0b10001,
        0b10001,
        0b10001
    },
    {
        // 13 : 'N'
        0b10001,
        0b11001,
        0b11001,
        0b10101,
        0b10011,
        0b10011,
        0b10001
    },
    {
        // 14 : 'O'
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    },
    {
        // 15 : 'P'
        0b11110,
        0b10001,
        0b10001,
        0b11110,
        0b10000,
        0b10000,
        0b10000
    },
    {
        // 16 : 'Q'
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b10010,
        0b01101
    },
    {
        // 17 : 'R'
        0b11110,
        0b10001,
        0b10001,
        0b11110,
        0b10100,
        0b10010,
        0b10001
    },
    {
        // 18 : 'S'
        0b01111,
        0b10000,
        0b10000,
        0b01110,
        0b00001,
        0b00001,
        0b11110
    },
    {
        // 19 : 'T'
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    },
    {
        // 20 : 'U'
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110
    },
    {
        // 21 : 'V'
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b00100
    },
    {
        // 22 : 'W'
        0b10001,
        0b10001,
        0b10001,
        0b10101,
        0b10101,
        0b10101,
        0b01010
    },
    {
        // 23 : 'X'
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b10001
    },
    {
        // 24 : 'Y'
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    },
    {
        // 25 : 'Z'
        0b11111,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000,
        0b11111
    },
    {
        // 26 : ' '
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    }
};

static const uint8_t font_numbers[10][FONT_HEIGHT] =
{
    // 0
    {
        0b01110,
        0b10001,
        0b10011,
        0b10101,
        0b11001,
        0b10001,
        0b01110
    },

    // 1
    {
        0b00100,
        0b01100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110
    },

    // 2
    {
        0b01110,
        0b10001,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b11111
    },

    // 3
    {
        0b11110,
        0b00001,
        0b00001,
        0b01110,
        0b00001,
        0b00001,
        0b11110
    },

    // 4
    {
        0b00010,
        0b00110,
        0b01010,
        0b10010,
        0b11111,
        0b00010,
        0b00010
    },

    // 5
    {
        0b11111,
        0b10000,
        0b10000,
        0b11110,
        0b00001,
        0b00001,
        0b11110
    },

    // 6
    {
        0b01110,
        0b10000,
        0b10000,
        0b11110,
        0b10001,
        0b10001,
        0b01110
    },

    // 7
    {
        0b11111,
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b01000,
        0b01000
    },

    // 8
    {
        0b01110,
        0b10001,
        0b10001,
        0b01110,
        0b10001,
        0b10001,
        0b01110
    },

    // 9
    {
        0b01110,
        0b10001,
        0b10001,
        0b01111,
        0b00001,
        0b00001,
        0b01110
    }
};

/* Colon ':' */
static const uint8_t font_symbol[FONT_HEIGHT] =
{
    0b00000,
    0b00100,
    0b00100,
    0b00000,
    0b00100,
    0b00100,
    0b00000
};


void font5x7_DrawChar(char character,
                       uint8_t start_row,
                       int8_t start_column) {

	const uint8_t* element;
	if (character >= 'A' && character <= 'Z') {
		element = font[character - 'A'];
	}
	else if (character >= '0' && character <= '9') {
		element = font_numbers[character - '0'];
	}
	else if (character == ':') {
		element = font_symbol;
	}
	else return; // Unsupported character or space

	for (uint8_t row = 0; row < FONT_HEIGHT; ++row) {
		for (uint8_t column = 0; column < FONT_WIDTH; ++column) {
			if (element[row] >> (4 - column) & 1)
			HUB75_SetPixel(row + start_row, column + start_column);
		}
	}

//	uint8_t index = character - 'A';
//	for (uint8_t row = 0; row < FONT_HEIGHT; ++row) {
//		for (uint8_t column = 0; column < FONT_WIDTH; ++column) {
//			if (font[index][row] >> (4 - column) & 1)
//			HUB75_SetPixel(row + start_row, column + start_column);
//		}
//	}
}

void font5x7_DrawStringAt(const char* text, int16_t start_col) {
    uint8_t x = 0;

    while (text[x] != '\0') {
        font5x7_DrawChar(text[x], 12, start_col);

        ++x;
        start_col += 6;
    }
}

/* This function takes text argument only text with size of 21 characters
 * between 2 word must be only 1 space*/
void font5x7_DrawStringCentered(const char* text) {
	uint8_t size = 0;
	uint8_t x = 0;
	while (text[x] != '\0') {
		(text[x] == ' ') ? (size += 2) : (size += 6);
		++x;
	}
	--size;
	if (size <= 63)  // 1 line
		font5x7_display_one_line(text, size, 12, (63 - size) / 2);


	else if (size > 63 && size <= 126)  // 2 lines
		font5x7_display_two_line(text, size);

	HUB75_SwapBuffers();
}

void font5x7_display_one_line(const char* text, const uint8_t size,
		const uint8_t start_row, uint8_t start_col) {

		uint8_t x = 0;
		while (x < size && text[x] != '\0') {
			if(text[x] == ' ') {
				++x; start_col += 2;
			}
			font5x7_DrawChar(text[x], start_row, start_col);
			++x;
			start_col += 6;
		}
}

void font5x7_display_two_line(const char* text, const uint8_t SIZE) {
		uint8_t start_row = 6;
		uint8_t start_col = 0;

		uint8_t x = 0;
		uint8_t space = 0; // another word on same line
		uint8_t last_word = 0; // when new line
		uint8_t another_word = 0; // another word on same line counting spaces
		char one_line[16] = {0};
		while (text[x] != '\0' && x < SIZE && start_row <= 24) {
			if(text[x + 1] == ' ' || text[x + 1] == '\0') {
				if ((((x - last_word + 1) * 6) - (4 * another_word)) <= 63) {
					if (!another_word) { // space = 0
						memcpy(one_line, text + last_word, x - last_word + 1);
					} else {
						memcpy((one_line + space), text + space + last_word, x - space - last_word + 1);
					}

					space = x - last_word + 1; // this is size of one line
					++x;
				}
				//count if next word can fit in one line
				if (keep_line(text, x + 1, last_word, another_word, SIZE)) {
					++x;
					++another_word;
					continue;
				}
			} else {
				++x; continue;
			}

			start_col = (63 - (space * 6 - 4 * another_word)) / 2 + 1;

			font5x7_display_one_line(one_line, space, start_row, start_col);

//			HUB75_SwapBuffers();

			// clean the array for next line
			memset(one_line, 0, sizeof(one_line));
			start_row += 12;
			space = 0;
			last_word = x + 1;
			another_word = 0;
			if (text[x + 1] != '\0') ++x;
		}
}

static uint8_t keep_line(const char* text, uint8_t index, const uint8_t LAST_WORD, const uint8_t ANOTHER_WORD, const uint8_t SIZE) {

	if (text[index + 1] == '\0' ||
	    text[index - 1] == '\0' ||
		text[index]     == '\0' ||
		index >= SIZE ) return 0;

	while(text[index] != ' ' && text[index] != '\0') {

		if (text[index + 1] == ' ' || text[index + 1] == '\0') {

			if ((!ANOTHER_WORD) && (((index - LAST_WORD) * 6) + 2) <= 63) {
				return 1;
			}
			else if (ANOTHER_WORD &&
					(((index - LAST_WORD + 1) * 6) - (4 * (ANOTHER_WORD + 1))) <= 63) {
				return 1;
			}
			else return 0;
		}
		++index;

	}
	return 0;
}

void HUB75_ReplaceTexts(const char* text[], const uint8_t SIZE) {

	static uint8_t current_text = 0;
	static uint8_t start = 1;
	if (start) {
		timer3s_flag = 1; start = 0;
	}
//	HUB75_Display();
	if (timer3s_flag) {
		timer3s_flag = 0;

		HUB75_ClearScreen();

		font5x7_DrawStringCentered(text[current_text]);
//		HUB75_SwapBuffers();

		++current_text;

		if (current_text >= SIZE) current_text = 0;
	}
}

//void HUB75_ScrollTexts(const char* text[], const uint8_t SIZE) {
//    static uint8_t current_text = 0;
//
//    // 0 = waiting, 1 = scrolling out, 2 = scrolling in
//    static uint8_t state = 0;
//
//    static int16_t x = 0; // position
//
//    uint8_t width = strlen(text[current_text]) * 6 - 1;
//
//    // Initialization
//    if (state == 0) {
//        x = (64 - width) / 2;
//        state = 3; // waiting
////        timer_counter = 0;
//
////        Timer_Reset3s_Scrolling();
//        timer3s_flag = 0;
//        timer3s_counter_scrolling = 0;
//
//
//        HUB75_ClearScreen();
//        drawStringAt(text[current_text], x);
//    }
//
//    // Start scrolling after 3 seconds
//    if (timer3s_flag && state == 3) {
//        timer3s_flag = 0;
//        state = 1;
//    }
//
//    // Move text every 50 ms
//    if (timer50ms_flag) {
//    	timer50ms_flag = 0;
//
//        if (state == 1) { // Scroll current word out
//            --x;
//
//            if (x + width < 0) {
//                ++current_text;
//
//                if (current_text >= SIZE) current_text = 0;
//
//                x = 64;
//                state = 2;
////                Timer_Reset3s_Scrolling();
//            }
//        }
//        else if (state == 2) { // Bring next word to center
//            --x;
//
//            width = strlen(text[current_text]) * 6 - 1;
//            int16_t center = (64 - width) / 2;
//
//            if (x <= center) {
//                x = center;
//                state = 3;
//
//                timer3s_flag = 0;
//                timer3s_counter_scrolling = 0;
////                Timer_Reset3s_Scrolling();
//            }
//        }
//
//        HUB75_ClearScreen();
//
//        drawStringAt(text[current_text], x);
//    }
//}

void HUB75_ScrollTexts(const char* text[], const uint8_t SIZE) {
	static uint8_t start = 0;
	static uint8_t scroll_out = 0;
	static uint8_t scroll_in = 0;
	static uint8_t wait_3s = 0;

	static uint8_t current = 0;

	static int16_t x = 0; // position

	uint8_t current_size = strlen(text[current]) * 6 - 1;

	if (start == 0) {
		x = (64 - current_size) / 2;
		start = 1; wait_3s = 1;
		timer3s_scrolling_flag = 0;
		timer3s_scrolling_counter = 0;
		HUB75_ClearScreen();
		drawStringAt(text[current], x);
		HUB75_SwapBuffers();
	}

	if (wait_3s && timer3s_scrolling_flag) {
		timer3s_scrolling_flag = 0;
		wait_3s = 0;
		scroll_out = 1;
	}

	if (scroll_out && timer50ms_flag) {
		timer50ms_flag = 0;

		--x;
		if (x + current_size < 0) {
			++current;
			if (current >= SIZE) current = 0;
			x = 64;
			scroll_out = 0; scroll_in = 1;
		}
	}

	if (scroll_in && timer50ms_flag) {
		timer50ms_flag = 0;
		--x;

		current_size = strlen(text[current]) * 6 - 1;
		int8_t center = (64 - current_size) / 2;
		if (x <= center) {
			x = center;
			wait_3s = 1;
			scroll_in = 0;
			timer3s_flag = 0;
			timer3s_scrolling_flag = 0;
			timer3s_scrolling_counter = 0;
		}
	}

	HUB75_ClearScreen();
	drawStringAt(text[current], x);
	HUB75_SwapBuffers();
}

void drawStringAt(const char* text, int16_t start_col) {
    uint8_t x = 0;

    while (text[x] != '\0')
    {
        font5x7_DrawChar(text[x], 12, start_col);

        ++x;
        start_col += 6;
    }
}
