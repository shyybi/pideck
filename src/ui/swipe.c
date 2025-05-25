#include "./swipe.h"

void swipe_init(SwipeState *swipe) {
    swipe->active = 0;
    swipe->start_x = -1;
    swipe->last_x = -1;
}

void swipe_handle_event(SwipeState *swipe, SDL_Event *event, int win_w) {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        swipe->active = 1;
        swipe->start_x = event->button.x;
        swipe->last_x = event->button.x;
    } else if (event->type == SDL_MOUSEMOTION && swipe->active) {
        swipe->last_x = event->motion.x;
    } else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && swipe->active) {
        swipe->active = 0;
    } else if (event->type == SDL_FINGERDOWN) {
        swipe->active = 1;
        swipe->start_x = (int)(event->tfinger.x * win_w);
        swipe->last_x = swipe->start_x;
    } else if (event->type == SDL_FINGERMOTION && swipe->active) {
        swipe->last_x = (int)(event->tfinger.x * win_w);
    } else if (event->type == SDL_FINGERUP && swipe->active) {
        swipe->active = 0;
    }
}

int swipe_get_direction(SwipeState *swipe, int threshold) {
    if (!swipe->active && swipe->start_x != -1 && swipe->last_x != -1) {
        int dx = swipe->last_x - swipe->start_x;
        swipe->start_x = -1;
        swipe->last_x = -1;
        if (dx < -threshold) return -1; // gauche
        if (dx > threshold) return 1;   // droite
    }
    return 0;
}
