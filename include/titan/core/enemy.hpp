// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_ENEMY_HPP_
#define TITAN_CORE_ENEMY_HPP_

#include "titan/utility/types.hpp"

struct list;
struct sfRenderWindow;
struct sfSprite;
struct sfTexture;

struct enemy {
        struct rect_f bounds;
        float dx = 0.0f;
        float dy = 0.0f;
        sfSprite *sprite = nullptr;
        sfTexture *texture = nullptr;
};

int enemy_create(char *, struct vector_2f, struct enemy **);
void enemy_destroy(void *);
void enemy_draw(struct list *, sfRenderWindow *);
void enemy_move(float, struct enemy *);
void enemy_update(float, struct enemy *);

#endif /* TITAN_CORE_ENEMY_HPP_ */
