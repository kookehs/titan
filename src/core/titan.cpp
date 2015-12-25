// Copyright 2015 Bill Lin. All rights reserved.


#include "titan/core/titan.hpp"

#include <windows.h>

void
game_state_destroy(struct game_state *state) {
        sfCircleShape_destroy(state->shape);
        state->shape = nullptr;
        sfRenderWindow_destroy(state->window);
        state->window = nullptr;
        free(state);
}

void
game_draw_circle(const sfCircleShape *shape, sfRenderWindow *window) {
        sfRenderWindow_drawCircleShape(window, shape, nullptr);
}

extern "C" struct game_state *
game_init(void) {
        struct game_state *state = (struct game_state *)malloc(sizeof(*state));
        sfVideoMode mode = { 200, 200, 32 };
        const char *title = "SFML works!";
        state->window = sfRenderWindow_create(mode, title, sfClose, nullptr);
        state->shape = sfCircleShape_create();
        sfCircleShape_setRadius(state->shape, 100.0f);
        sfCircleShape_setFillColor(state->shape, sfGreen);

        return state;
}

extern "C" int
game_update(struct game_state *state) {
        int exit = 0;

        if (sfRenderWindow_isOpen(state->window)) {
                sfEvent event;

                while (sfRenderWindow_pollEvent(state->window, &event)) {
                        if (event.type == sfEvtClosed) {
                                sfRenderWindow_close(state->window);
                                exit = 1;
                        }
                }

                game_window_clear(sfBlack, state->window);
                game_draw_circle(state->shape, state->window);
                game_window_display(state->window);
        }

        if (exit)
                game_state_destroy(state);

        return exit;
}

void
game_window_clear(const sfColor color, sfRenderWindow *window) {
        sfRenderWindow_clear(window, color);
}

void
game_window_display(sfRenderWindow *window) {
        sfRenderWindow_display(window);
}
