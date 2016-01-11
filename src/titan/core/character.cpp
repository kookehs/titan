// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/character.hpp"

#include <math.h>

#include "SFML/Graphics/RenderWindow.h"
#include "SFML/Graphics/Sprite.h"
#include "SFML/Graphics/Texture.h"
#include "SFML/Window/Keyboard.h"

#include "titan/utility/misc.hpp"

int
character_create(char *path, struct character *character) {
        character->x = 100.0f;
        character->y = 100.0f;
        character->dx = 0.0f;
        character->dy = 0.0f;
        character->acceleration = 20.0f;
        character->max_speed = 100.0f;
        character->texture = sfTexture_createFromFile(path, nullptr);
        character->width = sfTexture_getSize(character->texture).x;
        character->height = sfTexture_getSize(character->texture).y;

        if (character->texture == nullptr)
                return 0;

        character->sprite = sfSprite_create();
        sfSprite_setPosition(character->sprite, {character->x, character->y});
        sfSprite_setTexture(character->sprite, character->texture, true);

        if (character->sprite == nullptr)
                return 0;

        return 1;
}

void
character_destroy(struct character *character) {
        sfSprite_destroy(character->sprite);
        sfTexture_destroy(character->texture);
}

void
character_draw(struct character *character, sfRenderWindow *window) {
        sfRenderWindow_drawSprite(window, character->sprite, nullptr);
}

inline void
character_move(float delta, struct character *character) {
        float dx = character->dx * delta;
        float dy = character->dy * delta;
        character->x += dx;
        character->y += dy;
        sfSprite_move(character->sprite, {dx, dy});
        character->dx += sign(character->dx) * -1.0f;
        character->dy += sign(character->dy) * -1.0f;
}

void
character_process(struct character *character) {
        if (sfKeyboard_isKeyPressed(sfKeyW))
                character->dy -= character->acceleration;

        if (sfKeyboard_isKeyPressed(sfKeyS))
                character->dy += character->acceleration;

        if (sfKeyboard_isKeyPressed(sfKeyA))
                character->dx -= character->acceleration;

        if (sfKeyboard_isKeyPressed(sfKeyD))
                character->dx += character->acceleration;

        if (fabs(character->dx) > character->max_speed)
                character->dx = sign(character->dx) * character->max_speed;

        if (fabs(character->dy) > character->max_speed)
                character->dy = sign(character->dy) * character->max_speed;
}

void
character_update(float delta, struct character *character) {
        character_move(delta, character);
}
