#include <stdio.h>
#include <stdbool.h>

#include "lcd.h"
#include "logging.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (576)

static SDL_Window *window;
static SDL_Surface *surface;

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void lcd_init(void) {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "yobemag GB Emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_INPUT_FOCUS
    );

    surface = SDL_GetWindowSurface(window);
}

bool lcd_step(void) {
    SDL_Event e;
    const uint8_t* key_states;

	key_states = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            return true;
        }
    }

    if(key_states[SDL_SCANCODE_Q]) {
        return false;
    }


    if(key_states[SDL_SCANCODE_A]) {
        LOG_DEBUG("a");
    }

    return true;
}
