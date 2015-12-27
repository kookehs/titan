// Copyright 2015 Bill Lin. All rights reserved.

#ifndef TITAN_CORE_TITAN_HPP_
#define TITAN_CORE_TITAN_HPP_

#include <windows.h>

#include "SFML/Graphics/CircleShape.h"
#include "SFML/Graphics/RenderWindow.h"

struct game_state {
        sfCircleShape* shape = nullptr;
        sfRenderWindow* window = nullptr;
};

struct game_api {
        void (*init)(struct game_state *);
        int (*loop)(struct game_state *);
};

extern "C" const struct game_api api;

void game_state_destroy(struct game_state *);
void game_draw_cricle(const sfCircleShape *, sfRenderWindow *);
void game_init(struct game_state *);
int game_loop(struct game_state *);
void game_window_clear(const sfColor, sfRenderWindow *);
void game_window_display(sfRenderWindow *);

#endif  /* TITAN_CORE_TITAN_HPP_ */
