#include "dep/SDL2/SDL.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int ui_run(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
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
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("./font.ttf", 96);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int win_w = 800, win_h = 480;

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Obtenir l'heure
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[16];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface *time_surface = TTF_RenderUTF8_Blended(font, time_str, color);
        if (!time_surface) {
            printf("Erreur TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
            break;
        }
        SDL_Texture *time_texture = SDL_CreateTextureFromSurface(renderer, time_surface);

        SDL_Rect time_rect = {
            (win_w - time_surface->w) / 2,
            (win_h - time_surface->h) / 2,
            time_surface->w,
            time_surface->h
        };
        SDL_FreeSurface(time_surface);

        // Fond #343643
        SDL_SetRenderDrawColor(renderer, 52, 54, 67, 255);
        SDL_RenderClear(renderer);

        if (time_texture)
            SDL_RenderCopy(renderer, time_texture, NULL, &time_rect);

        SDL_RenderPresent(renderer);

        if (time_texture) SDL_DestroyTexture(time_texture);

        SDL_Delay(250);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
