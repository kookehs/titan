// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_CHARACTER_HPP_
#define TITAN_CORE_CHARACTER_HPP_

#include "titan/utility/types.hpp"

struct sfRenderWindow;
struct sfSprite;
struct sfTexture;

struct character {
        struct rect_f bounds;
        float dx = 0.0f;
        float dy = 0.0f;
        float acceleration = 20.0f;
        float max_speed = 100.0f;
        sfSprite *sprite = nullptr;
        sfTexture *texture = nullptr;
};

int character_create(char *, struct character *);
void character_destroy(struct character *);
void character_draw(struct character *, sfRenderWindow *);
void character_move(float, struct character *);
void character_process(struct character *);
void character_update(float, struct character *);

#endif /* TITAN_CORE_CHARACTER_HPP_ */
