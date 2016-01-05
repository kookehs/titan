// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_CHARACTER_HPP_
#define TITAN_CORE_CHARACTER_HPP_

#include "SFML/Graphics/Sprite.h"
#include "SFML/Graphics/Texture.h"

struct character {
        float x = 0.0f;
        float y = 0.0f;
        float dx = 0.0f;
        float dy = 0.0f;
        sfSprite *sprite = nullptr;
        sfTexture *texture = nullptr;
};

int character_create(char *, struct character *);
void character_destroy(struct character *);
void character_move(float, struct character *);
void character_process(struct character *);
void character_update(float, struct character *);

#endif /* TITAN_CORE_CHARACTER_HPP_ */
