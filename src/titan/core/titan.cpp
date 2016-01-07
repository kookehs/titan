// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/titan.hpp"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "SFML/Graphics/RenderWindow.h"
#include "SFML/System/Clock.h"
#include "SFML/Window/Event.h"

#include "titan/utility/hashmap.hpp"
#include "titan/utility/misc.hpp"
#include "titan/utility/physics.hpp"

int
game_add_enemy(struct game_state *state, char *path, float x, float y) {
        // TODO(bill): Relocate enemy related functions
        if (state->enemy_count == 0) {
                state->enemys = (struct enemy *)malloc(sizeof(*state->enemys));

                if (state->enemys == nullptr)
                        return 0;

                state->enemy_count = 1;
        } else {
                size_t size = (state->enemy_count + 1) * sizeof(struct enemy);
                struct enemy *tmp = (struct enemy *)realloc(state->enemys, size);

                if (tmp == nullptr)
                        return 0;

                state->enemys = tmp;
                ++state->enemy_count;
        }

        enemy_create(path, &state->enemys[state->enemy_count - 1], x, y);
        return 1;
}

inline void
game_destroy(struct game_state *state) {
        sfRenderWindow_destroy(state->window);
        state->window = nullptr;
        character_destroy(&state->character);
        enemy_destroy(state->enemys, state->enemy_count);
}

inline void
game_draw_sprite(sfSprite *sprite, sfRenderWindow *window) {
        sfRenderWindow_drawSprite(window, sprite, nullptr);
}

void
game_enumerate_enemy(struct game_state *state) {
        struct enemy *enemy = state->enemys;

        for (size_t i = 0; i < state->enemy_count; ++i, ++enemy) {
                game_draw_sprite(enemy->sprite, state->window);
        }
}

int
game_init(struct game_state *state) {
        if (state == nullptr)
                return 0;

        struct hashmap *map = hashmap_create(5);

        if (map == nullptr)
                return 0;

        game_load_config("../data/common.cfg", map);
        char buffer[UCHAR_MAX];
        hashmap_at(map, "WindowTitle", sizeof(buffer), buffer);
        char title[UCHAR_MAX];
        strncpy(title, buffer, sizeof(title));
        hashmap_at(map, "WindowHeight", sizeof(buffer), buffer);
        uint32_t height = strtol(buffer, nullptr, 10);
        hashmap_at(map, "WindowWidth", sizeof(buffer), buffer);
        uint32_t width = strtol(buffer, nullptr, 10);
        hashmap_at(map, "BitDepth", sizeof(buffer), buffer);
        uint32_t bit_depth = strtol(buffer, nullptr, 10);
        hashmap_at(map, "FrameRate", sizeof(buffer), buffer);
        uint32_t frame_rate = strtol(buffer, nullptr, 10);
        hashmap_destroy(map);

        sfVideoMode mode = {width, height, bit_depth};
        state->window = sfRenderWindow_create(mode, title, sfClose, nullptr);
        state->clock = sfClock_create();
        state->update_time = sfTime_Zero;
        state->delta = 1.0f / frame_rate;
        state->frame_time = state->delta * 1000000;

        character_create("../data/textures/character.png", &state->character);
        // enemy_create("../data/textures/enemy.png", &state->enemys);
        game_add_enemy(state, "../data/textures/enemy.png", 200, 200);
        game_add_enemy(state, "../data/textures/enemy.png", 400, 400);
        return 1;
}

int
game_load_config(char *path, struct hashmap *map) {
        FILE *input_file;
        input_file = fopen(path, "r");

        if (input_file == nullptr)
                return 0;

        char buffer[UCHAR_MAX] = {0};
        char key[UCHAR_MAX] = {0};
        char value[UCHAR_MAX] = {0};

        while (fgets(buffer, UCHAR_MAX, input_file)) {
                if (buffer[0] == '[')
                        continue;

                char *save = nullptr;
                char *token = strtok_s(buffer, "=", &save);
                strncpy(key, token, sizeof(key));
                token = strtok_s(nullptr, "=", &save);
                trim(&token, "\n");
                strncpy(value, token, sizeof(value));
                hashmap_insert(key, value, map);
        }

        fclose(input_file);
        return 1;
}

int
game_loop(struct game_state *state) {
        int error_code = 0;

        if (sfRenderWindow_isOpen(state->window)) {
                sfTime delta_time = sfClock_restart(state->clock);
                state->update_time.microseconds += delta_time.microseconds;

                while (state->update_time.microseconds > state->frame_time) {
                        state->update_time.microseconds -= state->frame_time;
                        error_code = game_process(state);

                        if (error_code)
                                return error_code;

                        game_update(state);
                }

                game_window_clear(sfBlack, state->window);
                game_window_render(state);
                game_window_display(state->window);
        }

        if (error_code)
                game_destroy(state);

        return error_code;
}

int
game_process(struct game_state *state) {
        sfEvent event;

        // TODO(bill): Figure out why the following line causes a crash
        //             Could it be because of varying CRT lbrary?
        while (sfRenderWindow_pollEvent(state->window, &event)) {
                if (event.type == sfEvtClosed) {
                        sfRenderWindow_close(state->window);
                        return 1;
                } else if (event.type == sfEvtKeyPressed) {
                        if (event.key.code == sfKeyEscape) {
                                sfRenderWindow_close(state->window);
                                return 1;
                        }
                } else if (event.type == sfEvtGainedFocus) {
                        state->window_focused = true;
                } else if (event.type == sfEvtLostFocus) {
                        state->window_focused = false;
                }
        }

        if (state->window_focused)
                character_process(&state->character);

        return 0;
}

void
game_resolve_collision(struct game_state *state) {
        struct aabb a;
        a.x = state->character.x + state->character.dx * state->delta;
        a.y = state->character.y + state->character.dy * state->delta;
        a.width = state->character.width;
        a.height = state->character.height;

        /*
        struct aabb b;
        b.x = state->enemys.x + state->enemys.dx * state->delta;
        b.y = state->enemys.y + state->enemys.dy * state->delta;
        b.width = state->enemys.width;
        b.height = state->enemys.height;

        if (physics_aabb(&a, &b)) {
                state->character.dx = 0;
                state->character.dy = 0;
                state->enemys.dx = 0;
                state->enemys.dy = 0;
        }
        */
}

inline void
game_update(struct game_state *state) {
        game_resolve_collision(state);
        character_update(state->delta, &state->character);
}

inline void
game_window_clear(sfColor color, sfRenderWindow *window) {
        sfRenderWindow_clear(window, color);
}

inline void
game_window_render(struct game_state *state) {
        game_draw_sprite(state->character.sprite, state->window);
        // game_draw_sprite(state->enemys.sprite, state->window);
        game_enumerate_enemy(state);
}

inline void
game_window_display(sfRenderWindow *window) {
        sfRenderWindow_display(window);
}

extern "C" const struct game_api api = {
        game_init,
        game_loop
};
