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

#include "titan/utility/list.hpp"

int
enemy_create(char *path, struct vector_2f point, struct enemy **enemy) {
        *enemy = (struct enemy *)malloc(sizeof(*(*enemy)));
        (*enemy)->bounds.x = point.x;
        (*enemy)->bounds.y = point.y;
        (*enemy)->dx = 0.0f;
        (*enemy)->dy = 0.0f;
        (*enemy)->texture = sfTexture_createFromFile(path, nullptr);
        (*enemy)->bounds.width = sfTexture_getSize((*enemy)->texture).x;
        (*enemy)->bounds.height = sfTexture_getSize((*enemy)->texture).y;

        if ((*enemy)->texture == nullptr)
                return 0;

        (*enemy)->sprite = sfSprite_create();
        float x = (*enemy)->bounds.x;
        float y = (*enemy)->bounds.y;
        sfSprite_setPosition((*enemy)->sprite, {x, y});
        sfSprite_setTexture((*enemy)->sprite, (*enemy)->texture, true);

        if ((*enemy)->sprite == nullptr)
                return 0;

        return 1;
}

void
enemy_destroy(void *data) {
        struct enemy *enemy = (struct enemy *)data;
        sfSprite_destroy(enemy->sprite);
        sfTexture_destroy(enemy->texture);
}

void
enemy_draw(struct list *enemys, sfRenderWindow *window) {
        struct list_node *current = enemys->head;

        while (current != nullptr) {
                struct enemy *enemy = (struct enemy *)current->data;
                sfRenderWindow_drawSprite(window, enemy->sprite, nullptr);
                current = current->next;
        }
}

inline void
enemy_move(float delta, struct enemy *enemy) {
        float dx = enemy->dx * delta;
        float dy = enemy->dy * delta;
        enemy->bounds.x += dx;
        enemy->bounds.y += dy;
        sfSprite_move(enemy->sprite, {dx, dy});
        enemy->dx = 0.0f;
        enemy->dy = 0.0f;
}

inline void
enemy_update(float delta, struct enemy *enemy) {
        enemy_move(delta, enemy);
}
