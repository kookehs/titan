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

#include "SFML/Graphics/Color.h"
#include "SFML/System/Time.h"

#include "titan/core/character.hpp"
#include "titan/core/enemy.hpp"

struct hashmap;
struct sfClock;
struct sfRenderWindow;

struct game_api {
        int (*init)(struct game_state *);
        int (*loop)(struct game_state *);
};

struct game_state {
        struct character character;
        sfClock *clock = nullptr;
        float delta = 0.0f;
        size_t enemy_count = 0;
        struct enemy *enemys;
        sfInt64 frame_time = 0.0f;
        sfTime update_time = {};
        sfRenderWindow *window = nullptr;
        bool window_focused = true;
};

extern "C" const struct game_api api;

int game_add_enemy(struct game_state *, char *, float, float);
void game_destroy(struct game_state *);
void game_draw_sprite(sfSprite *, sfRenderWindow *);
void game_enumerate_enemy(struct game_state *);
int game_init(struct game_state *);
int game_load_config(char *path, struct hashmap *);
int game_loop(struct game_state *);
int game_process(struct game_state *);
void game_resolve_collision(struct game_state *);
void game_update(struct game_state *);
void game_window_clear(sfColor, sfRenderWindow *);
void game_window_render(struct game_state *);
void game_window_display(sfRenderWindow *);

#endif /* TITAN_CORE_TITAN_HPP_ */
