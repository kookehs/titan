// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/titan.hpp"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "SFML/Graphics/RenderWindow.h"
#include "SFML/System/Clock.h"
#include "SFML/Window/Event.h"

#include "titan/core/enemy.hpp"
#include "titan/utility/hashmap.hpp"
#include "titan/utility/list.hpp"
#include "titan/utility/misc.hpp"
#include "titan/utility/physics.hpp"
#include "titan/utility/types.hpp"

inline void
game_destroy(struct game_state *state) {
        sfClock_destroy(state->clock);
        sfRenderWindow_destroy(state->window);
        character_destroy(&state->character);
        list_destroy(state->enemys);
}

int
game_init(struct game_state *state) {
        if (state == nullptr)
                return 0;

        struct hashmap *map;
        hashmap_create(5, &map);

        if (map == nullptr)
                return 0;

        game_load_config("../data/common.cfg", map);
        char buffer[UCHAR_MAX] = {0};
        hashmap_at(map, "WindowTitle", sizeof(buffer), buffer);
        char title[UCHAR_MAX] = {0};
        strncpy(title, buffer, sizeof(title));
        hashmap_at(map, "WindowHeight", sizeof(buffer), buffer);
        uint32_t height = strtol(buffer, nullptr, 10);
        hashmap_at(map, "WindowWidth", sizeof(buffer), buffer);
        uint32_t width = strtol(buffer, nullptr, 10);
        hashmap_at(map, "BitDepth", sizeof(buffer), buffer);
        uint32_t bit_depth = strtol(buffer, nullptr, 10);
        hashmap_at(map, "FrameRate", sizeof(buffer), buffer);
        uint32_t frame_rate = strtol(buffer, nullptr, 10);
        hashmap_at(map, "VSync", sizeof(buffer), buffer);
        bool vsync = strtol(buffer, nullptr, 10);
        hashmap_destroy(map);

        sfVideoMode mode = {width, height, bit_depth};
        state->window = sfRenderWindow_create(mode, title, sfClose, nullptr);

        if (vsync)
                sfRenderWindow_setVerticalSyncEnabled(state->window, true);

        state->clock = sfClock_create();
        state->update_time = sfTime_Zero;
        state->delta = 1.0f / frame_rate;
        state->frame_time = state->delta * 1000000;

        character_create("../data/textures/character.png", &state->character);

        list_create(sizeof(struct enemy), enemy_destroy, &state->enemys);
        char *enemy_texutre = "../data/textures/enemy_big.png";
        struct vector_2f a;
        a.x = 200;
        a.y = 200;
        struct enemy *e = nullptr;
        enemy_create(enemy_texutre, a, &e);
        list_push_back(e, state->enemys);

        struct vector_2f b;
        b.x = 400;
        b.y = 400;
        struct enemy *f = nullptr;
        enemy_create(enemy_texutre, b, &f);
        list_push_back(f, state->enemys);

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
                game_window_draw(state);
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
        // TODO(bill): Handle collision in an more optimal manner
        float character_dx = state->character.dx;
        float character_dy = state->character.dy;

        struct rect_f a = state->character.bounds;
        a.x += character_dx * state->delta;
        a.y += character_dy * state->delta;

        struct list_node *current = state->enemys->head;

        while (current != nullptr) {
                struct enemy *enemy = (struct enemy *)current->data;

                struct rect_f b = enemy->bounds;
                b.x += enemy->dx * state->delta;
                b.y += enemy->dy * state->delta;

                if (physics_aabb_intersects(&a, &b)) {
                        struct rect_f c = state->character.bounds;
                        enum side hit = physics_aabb_hit(&a, &b, &c);

                        if (hit == bottom || hit == top) {
                                state->character.dy *= -1;
                        } else if (hit == left || hit == right) {
                                state->character.dx *= -1;
                        }

                        enemy->dx = 0;
                        enemy->dy = 0;
                }

                current = current->next;
        }
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
game_window_display(sfRenderWindow *window) {
        sfRenderWindow_display(window);
}

inline void
game_window_draw(struct game_state *state) {
        character_draw(&state->character, state->window);
        enemy_draw(state->enemys, state->window);
}

extern "C" const struct game_api api = {
        game_init,
        game_loop
};
