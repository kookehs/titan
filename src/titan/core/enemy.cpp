// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/enemy.hpp"

#include <stdlib.h>

#include "SFML/Graphics/RenderWindow.h"
#include "SFML/Graphics/Sprite.h"
#include "SFML/Graphics/Texture.h"

int
enemy_create(char *path, float x, float y, struct enemy *enemy) {
        enemy->x = x;
        enemy->y = y;
        enemy->dx = 0.0f;
        enemy->dy = 0.0f;
        enemy->texture = sfTexture_createFromFile(path, nullptr);
        enemy->width = sfTexture_getSize(enemy->texture).x;
        enemy->height = sfTexture_getSize(enemy->texture).y;

        if (enemy->texture == nullptr)
                return 0;

        enemy->sprite = sfSprite_create();
        sfSprite_setPosition(enemy->sprite, {enemy->x, enemy->y});
        sfSprite_setTexture(enemy->sprite, enemy->texture, true);

        if (enemy->sprite == nullptr)
                return 0;

        return 1;
}

inline void
enemy_destroy(struct enemy *enemy) {
        sfSprite_destroy(enemy->sprite);
        sfTexture_destroy(enemy->texture);
}

inline void
enemy_move(float delta, struct enemy *enemy) {
        float dx = enemy->dx;
        float dy = enemy->dy;
        enemy->x += dx;
        enemy->y += dy;
        sfSprite_move(enemy->sprite, {dx * delta, dy * delta});
        enemy->dx = 0.0f;
        enemy->dy = 0.0f;
}

inline void
enemy_update(float delta, struct enemy *enemy) {
        enemy_move(delta, enemy);
}

int
enemy_list_add(char *path, float x, float y, struct enemy_list *list) {
        if (list->size == 0) {
                list->enemys = (struct enemy *)malloc(sizeof(*list->enemys));

                if (list->enemys == nullptr)
                        return 0;

                list->size = 1;
        } else {
                size_t size = (list->size + 1) * sizeof(struct enemy);
                struct enemy *tmp = (struct enemy *)realloc(list->enemys, size);

                if (tmp == nullptr)
                        return 0;

                list->enemys = tmp;
                ++list->size;
        }

        enemy_create(path, x, y, &list->enemys[list->size - 1]);
        return 1;
}

void
enemy_list_destroy(struct enemy_list *list) {
        struct enemy *enemy = list->enemys;

        for (size_t i = 0; i < list->size; ++i, ++enemy) {
                enemy_destroy(enemy);
        }
}

void
enemy_list_draw(struct enemy_list *list, sfRenderWindow *window) {
        struct enemy *enemy = list->enemys;

        for (size_t i = 0; i < list->size; ++i, ++enemy) {
                sfRenderWindow_drawSprite(window, enemy->sprite, nullptr);
        }
}
