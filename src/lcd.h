#ifndef YOBEMAG_LCD_H
#define YOBEMAG_LCD_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "err_codes.h"

void lcd_init(void);
bool lcd_step(void);

#endif //YOBEMAG_LCD_H
