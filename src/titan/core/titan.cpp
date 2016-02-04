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
#include "titan/utility/quadtree.hpp"
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
        hashmap_create(UCHAR_MAX, 5, &map);

        if (map == nullptr)
                return 0;

        game_load_config("../data/common.cfg", map);
        char buffer[UCHAR_MAX] = {0};
        hashmap_at(map, "WindowTitle", buffer);
        char title[UCHAR_MAX] = {0};
        strncpy(title, buffer, sizeof(title));
        hashmap_at(map, "WindowHeight", buffer);
        uint32_t height = strtol(buffer, nullptr, 10);
        hashmap_at(map, "WindowWidth", buffer);
        uint32_t width = strtol(buffer, nullptr, 10);
        hashmap_at(map, "BitDepth", buffer);
        uint32_t bit_depth = strtol(buffer, nullptr, 10);
        hashmap_at(map, "FrameRate", buffer);
        uint32_t frame_rate = strtol(buffer, nullptr, 10);
        hashmap_at(map, "VSync", buffer);
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

        struct list_info info;
        info.size_of_data = sizeof(struct enemy);
        info.destroy = enemy_destroy;
        list_create(info, &state->enemys);
        char *enemy_texutre = "../data/textures/enemy_big.png";
        struct vector_2f a;
        a.x = 200;
        a.y = 200;
        struct enemy *e = nullptr;
        enemy_create(enemy_texutre, a, &e);
        list_push_back(e, state->enemys);

        struct vector_2f b;
        b.x = 600;
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
        struct list_info list_info;
        list_info.destroy = nullptr;
        list_info.size_of_data = sizeof(struct quadtree_item);
        sfVector2u window_size = sfRenderWindow_getSize(state->window);
        struct rect_f rect;
        rect.x = 0.0f;
        rect.y = 0.0f;
        rect.width = window_size.x;
        rect.height = window_size.y;
        struct quadtree_info info;
        info.depth = 0;
        info.rect = rect;
        info.list_info = list_info;
        struct quadtree *tree = nullptr;
        quadtree_create(info, &tree);

        struct list_node *enemies = state->enemys->head;

        while (enemies != nullptr) {
                struct enemy *enemy = (struct enemy *)enemies->data;
                struct rect_f b = enemy->bounds;
                b.x += enemy->dx * state->delta;
                b.y += enemy->dy * state->delta;
                struct quadtree_item item;
                item.data = enemy;
                item.rect = b;
                quadtree_insert(item, tree);
                enemies = enemies->next;
        }

        struct rect_f a = state->character.bounds;
        a.x += state->character.dx * state->delta;
        a.y += state->character.dy * state->delta;

        struct list *nearby_enemies = nullptr;
        list_create(list_info, &nearby_enemies);
        quadtree_at(nearby_enemies, a, *tree);

        struct list_node *nearby = nearby_enemies->head;

        while (nearby != nullptr) {
                struct list_node *node = (struct list_node *)nearby->data;
                struct quadtree_item *item = (struct quadtree_item *)node->data;
                struct enemy *enemy = (struct enemy *)item->data;
                struct rect_f b = enemy->bounds;

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

                nearby = nearby->next;
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
