// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_TITAN_HPP_
#define TITAN_CORE_TITAN_HPP_

#include <windows.h>

#include "SFML/Graphics/CircleShape.h"
#include "SFML/Graphics/RenderWindow.h"
#include "SFML/System/Clock.h"
#include "SFML/System/Time.h"

#include "titan/utility/hashmap.hpp"

struct game_api {
        void (*init)(struct game_state *);
        int (*loop)(struct game_state *);
};

struct game_state {
        sfClock *clock = nullptr;
        sfTime update_time = sfTime_Zero;
        sfInt64 frame_time = 0.0f;
        sfCircleShape *shape = nullptr;
        sfRenderWindow *window = nullptr;
};

extern "C" const struct game_api api;

void game_state_destroy(struct game_state *);
void game_draw_cricle(sfCircleShape *, sfRenderWindow *);
void game_init(struct game_state *);
int game_load_config(char *path, struct hashmap *);
int game_loop(struct game_state *);
int game_process(struct game_state *);
void game_window_clear(sfColor, sfRenderWindow *);
void game_window_display(sfRenderWindow *);

#endif  /* TITAN_CORE_TITAN_HPP_ */
