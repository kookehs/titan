// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/character.hpp"

#include "SFML/Window/Keyboard.h"

int
character_create(char *path, struct character *character) {
        character->x = 100.0f;
        character->y = 100.0f;
        character->dx = 0.0f;
        character->dy = 0.0f;
        character->texture = sfTexture_createFromFile(path, nullptr);

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

inline void
character_move(float delta, struct character *character) {
        float dx = character->dx;
        float dy = character->dy;
        character->x += dx;
        character->y += dy;
        sfSprite_move(character->sprite, {dx * delta, dy * delta});
        character->dx = 0.0f;
        character->dy = 0.0f;
}

void
character_process(struct character *character) {
        if (sfKeyboard_isKeyPressed(sfKeyW))
                character->dy += -50.0f;

        if (sfKeyboard_isKeyPressed(sfKeyS))
                character->dy += 50.0f;

        if (sfKeyboard_isKeyPressed(sfKeyA))
                character->dx += -50.0f;

        if (sfKeyboard_isKeyPressed(sfKeyD))
                character->dx += 50.0f;
}

void
character_update(float delta, struct character *character) {
        character_move(delta, character);
}
