// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/enemy.hpp"

int
enemy_create(char *path, struct enemy *enemy) {
        enemy->x = 250.0f;
        enemy->y = 250.0f;
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

void
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

void
enemy_update(float delta, struct enemy *enemy) {
        enemy_move(delta, enemy);
}
