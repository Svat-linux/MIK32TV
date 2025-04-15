#include "main.h"
#include "font.h"

uint8_t buffer[WIDTH][HEIGHT / 8];    

int main()
{
    SystemClock_Config();

    GPIO_Init();

    clear_screen(0);
    draw_pixel(10, 10, 1);
    draw_text(20, 30, "Hello, Mik32!");

    while (1)
    {
        for (uint8_t line = 0; line < HEIGHT; line++) {
            output_scanline(line);
        }

        for (uint8_t i = 0; i < 166; i++) {
            HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, 0);
            HAL_DelayUs(4.7);
            HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, 1);
            HAL_DelayUs(58.8);
        }

        HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, 0);

        HAL_DelayMs(500);
    }
}

void SystemClock_Config(void)
{
    PCC_InitTypeDef PCC_OscInit = {0};

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider = 0;
    PCC_OscInit.APBMDivider = 0;
    PCC_OscInit.APBPDivider = 0;
    PCC_OscInit.HSI32MCalibrationValue = 128;
    PCC_OscInit.LSI32KCalibrationValue = 8;
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;
    HAL_PCC_Config(&PCC_OscInit);
}

void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_PCC_GPIO_1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    
    HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
}

void draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= WIDTH || y >= HEIGHT) return;
    uint8_t mask = 1 << (7 - (x % 8));
    if (color) {
        buffer[y][x / 8] |= mask;
    } else {
        buffer[y][x / 8] &= ~mask;
    }
}

void clear_screen(uint8_t color) {
    memset(buffer, color ? 0xFF : 0x00, sizeof(buffer));
}

void draw_char(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 127) return;
    const uint8_t *glyph = font[c - 32];
    for (uint8_t dy = 0; dy < CHAR_H; dy++) {
        for (uint8_t dx = 0; dx < CHAR_W; dx++) {
            if (glyph[dx] & (1 << dy)) {
                draw_pixel(x + dx, y + dy, 1);
            }
        }
    }
}

void draw_text(uint8_t x, uint8_t y, const char *str) {
    while (*str) {
        draw_char(x, y, *str++);
        x += CHAR_W;
    }
}

void output_scanline(uint8_t line) {
    HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, 0);
    HAL_DelayUs(4.7);
    HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, 1);

    for (uint8_t x = 0; x < WIDTH / 8; x++) {
        uint8_t pixels = buffer[line][x];
        for (uint8_t bit = 0; bit < 8; bit++) {
            HAL_GPIO_WritePin(GPIO_1, GPIO_PIN_3, (pixels >> (7 - bit)) & 1);
            HAL_DelayUs(0.5);
        }
    }
}