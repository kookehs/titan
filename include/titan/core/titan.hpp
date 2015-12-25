// Copyright 2015 Bill Lin. All rights reserved.

#ifndef TITAN_CORE_TITAN_HPP_
#define TITAN_CORE_TITAN_HPP_

#include <windows.h>

#include "SFML/Graphics/CircleShape.h"
#include "SFML/Graphics/RenderWindow.h"

using game_draw_cricle_ptr = void (*)(const sfCircleShape *, sfRenderWindow *);
using game_init_ptr = struct game_state *(*)(void);
using game_update_ptr = bool (*)(struct game_state *);
using game_window_clear_ptr = void (*)(const sfColor, sfRenderWindow *);
using game_window_display_ptr = void (*)(sfRenderWindow *);

struct game_state {
        sfCircleShape* shape = nullptr;
        sfRenderWindow* window = nullptr;
};

struct game_api {
        struct game_state *state;
        HMODULE handle;
        FILETIME last_write_time;
        game_init_ptr game_init = nullptr;
        game_update_ptr game_update = nullptr;
};

void game_destroy(struct game_state *);
void game_draw_cricle(const sfCircleShape *, sfRenderWindow *);
extern "C" struct game_state *game_init(void);
extern "C" int game_update(struct game_state *);
void game_window_clear(const sfColor, sfRenderWindow *);
void game_window_display(sfRenderWindow *);

#endif  /* TITAN_CORE_TITAN_HPP_ */
