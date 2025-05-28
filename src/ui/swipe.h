#ifndef SWIPE_H
#define SWIPE_H

#include "dep/SDL2/SDL.h"

typedef struct {
    int active;
    int start_x;
    int last_x;
    int key_dir;
} SwipeState;

void swipe_init(SwipeState *swipe);
void swipe_handle_event(SwipeState *swipe, SDL_Event *event, int win_w);
int swipe_get_direction(SwipeState *swipe, int threshold);

#endif
