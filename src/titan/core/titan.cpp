// Copyright 2015 Bill Lin. All rights reserved.

#include "titan/core/titan.hpp"

#include <windows.h>

inline void
game_state_destroy(struct game_state *state) {
        sfCircleShape_destroy(state->shape);
        state->shape = nullptr;
        sfRenderWindow_destroy(state->window);
        state->window = nullptr;
}

inline  void
game_draw_circle(const sfCircleShape *shape, sfRenderWindow *window) {
        sfRenderWindow_drawCircleShape(window, shape, nullptr);
}

void
game_init(struct game_state *state) {
        sfVideoMode mode = { 200, 200, 32 };
        const char *title = "SFML works!";
        state->window = sfRenderWindow_create(mode, title, sfClose, nullptr);
        state->shape = sfCircleShape_create();
        sfCircleShape_setRadius(state->shape, 100.0f);
        sfCircleShape_setFillColor(state->shape, sfGreen);
}

int
game_loop(struct game_state *state) {
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

inline void
game_window_clear(const sfColor color, sfRenderWindow *window) {
        sfRenderWindow_clear(window, color);
}

inline void
game_window_display(sfRenderWindow *window) {
        sfRenderWindow_display(window);
}

extern "C" const struct game_api api = {
        game_init,
        game_loop
};
