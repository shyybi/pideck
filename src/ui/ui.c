#include "dep/SDL2/SDL.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "swipe.h"

#define SWIPE_THRESHOLD 80
char g_track_title[128] = "Track Title";
char g_artist_name[128] = "Artist Name";

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
    int current_screen = 0; 

    SwipeState swipe;
    swipe_init(&swipe);

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            swipe_handle_event(&swipe, &event, win_w);
        }

        int swipe_dir = swipe_get_direction(&swipe, SWIPE_THRESHOLD);
        if (swipe_dir == -1 && current_screen < 3) {
            current_screen++;
        } else if (swipe_dir == 1 && current_screen > 0) {
            current_screen--;
        }

        SDL_SetRenderDrawColor(renderer,
            current_screen == 0 ? 52 : 0,
            current_screen == 0 ? 54 : 0,
            current_screen == 0 ? 67 : 0,
            255);
        SDL_RenderClear(renderer);

        if (current_screen == 0) {
            // Date and hour (First Window)
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            char time_str[16];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
            char date_str[32];
            strftime(date_str, sizeof(date_str), "%b %d %Y", tm_info);

            SDL_Color color = {255, 255, 255, 255};
            TTF_Font *font_date = TTF_OpenFont("./font.ttf", 48);

            SDL_Surface *time_surface = TTF_RenderUTF8_Blended(font, time_str, color);
            SDL_Surface *date_surface = font_date ? TTF_RenderUTF8_Blended(font_date, date_str, color) : NULL;

            SDL_Texture *time_texture = NULL;
            SDL_Texture *date_texture = NULL;
            if (time_surface)
                time_texture = SDL_CreateTextureFromSurface(renderer, time_surface);
            if (date_surface)
                date_texture = SDL_CreateTextureFromSurface(renderer, date_surface);

            SDL_Rect time_rect = {0};
            SDL_Rect date_rect = {0};
            if (time_surface) {
                time_rect.w = time_surface->w;
                time_rect.h = time_surface->h;
                time_rect.x = (win_w - time_rect.w) / 2;
                time_rect.y = (win_h - time_rect.h) / 2 - 30;
                SDL_FreeSurface(time_surface);
            }
            if (date_surface) {
                date_rect.w = date_surface->w;
                date_rect.h = date_surface->h;
                date_rect.x = (win_w - date_rect.w) / 2;
                date_rect.y = time_rect.y + time_rect.h + 5;
                SDL_FreeSurface(date_surface);
            }

            if (time_texture)
                SDL_RenderCopy(renderer, time_texture, NULL, &time_rect);
            if (date_texture)
                SDL_RenderCopy(renderer, date_texture, NULL, &date_rect);

            if (time_texture) SDL_DestroyTexture(time_texture);
            if (date_texture) SDL_DestroyTexture(date_texture);
            if (font_date) TTF_CloseFont(font_date);
        } else if (current_screen == 1) {
            // Audio player (Second Window)
            const char *title = g_track_title;
            const char *artist = g_artist_name;
            const char *header = "Audio Player";
            SDL_Color color = {255, 255, 255, 255};

            TTF_Font *font_header = TTF_OpenFont("./font.ttf", 36);
            TTF_Font *font_title = TTF_OpenFont("./font.ttf", 48);
            TTF_Font *font_artist = TTF_OpenFont("./font.ttf", 32);

            SDL_Surface *header_surf = font_header ? TTF_RenderUTF8_Blended(font_header, header, color) : NULL;
            SDL_Texture *header_tex = header_surf ? SDL_CreateTextureFromSurface(renderer, header_surf) : NULL;
            SDL_Rect header_rect = {0};
            if (header_surf) {
                header_rect.w = header_surf->w;
                header_rect.h = header_surf->h;
                header_rect.x = (win_w - header_rect.w) / 2;
                header_rect.y = 40;
                SDL_FreeSurface(header_surf);
            }

            // Title
            SDL_Surface *title_surf = font_title ? TTF_RenderUTF8_Blended(font_title, title, color) : NULL;
            SDL_Texture *title_tex = title_surf ? SDL_CreateTextureFromSurface(renderer, title_surf) : NULL;
            SDL_Rect title_rect = {0};
            if (title_surf) {
                title_rect.w = title_surf->w;
                title_rect.h = title_surf->h;
                title_rect.x = (win_w - title_rect.w) / 2;
                title_rect.y = win_h / 2 - title_rect.h;
                SDL_FreeSurface(title_surf);
            }

            // Artist
            SDL_Surface *artist_surf = font_artist ? TTF_RenderUTF8_Blended(font_artist, artist, color) : NULL;
            SDL_Texture *artist_tex = artist_surf ? SDL_CreateTextureFromSurface(renderer, artist_surf) : NULL;
            SDL_Rect artist_rect = {0};
            if (artist_surf) {
                artist_rect.w = artist_surf->w;
                artist_rect.h = artist_surf->h;
                artist_rect.x = (win_w - artist_rect.w) / 2;
                artist_rect.y = title_rect.y + title_rect.h + 16;
                SDL_FreeSurface(artist_surf);
            }

            if (header_tex) {
                SDL_RenderCopy(renderer, header_tex, NULL, &header_rect);
                SDL_DestroyTexture(header_tex);
            }
            if (title_tex) {
                SDL_RenderCopy(renderer, title_tex, NULL, &title_rect);
                SDL_DestroyTexture(title_tex);
            }
            if (artist_tex) {
                SDL_RenderCopy(renderer, artist_tex, NULL, &artist_rect);
                SDL_DestroyTexture(artist_tex);
            }

            if (font_header) TTF_CloseFont(font_header);
            if (font_title) TTF_CloseFont(font_title);
            if (font_artist) TTF_CloseFont(font_artist);
        } else {
            // Audio player (Second Window)
            const char *msg = "Hello World";
            SDL_Color color = {255, 255, 255, 255};
            SDL_Surface *surf = TTF_RenderUTF8_Blended(font, msg, color);
            if (surf) {
                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_Rect rect = { (win_w-surf->w)/2, (win_h-surf->h)/2, surf->w, surf->h };
                SDL_FreeSurface(surf);
                if (tex) {
                    SDL_RenderCopy(renderer, tex, NULL, &rect);
                    SDL_DestroyTexture(tex);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
