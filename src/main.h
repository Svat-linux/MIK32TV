#include "mik32_hal_pcc.h"
#include "mik32_hal_gpio.h"
#include <string.h>

#define WIDTH   128
#define HEIGHT  96
#define CHAR_W  6
#define CHAR_H  8

void SystemClock_Config();
void GPIO_Init();
void draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void clear_screen(uint8_t color);
void draw_char(uint8_t x, uint8_t y, char c);
void draw_text(uint8_t x, uint8_t y, const char *str);
void output_scanline(uint8_t line);