//#include <SDL2/SDL.h>
#include </opt/homebrew/opt/sdl2/include/SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

int ui_run(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
	
    SDL_Window *window = SDL_CreateWindow(
        "PiDeck",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 480,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return (1);
    }

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return (0);
}
