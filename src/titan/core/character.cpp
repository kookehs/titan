// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/core/character.hpp"

int
character_create(char *path, struct character *character) {
        character->x = 100.0f;
        character->y = 100.0f;
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
character_move(float dx, float dy, struct character *character) {
        character->x += dx;
        character->y += dy;
        sfSprite_move(character->sprite, {dx, dy});
}

void
character_process(float delta, sfKeyCode key, struct character *character) {
        if (key == sfKeyW) {
                character_move(0.0f, -50.0f * delta, character);
        } else if (key == sfKeyS) {
                character_move(0.0f, 50.0f * delta, character);
        } else if (key == sfKeyA) {
                character_move(-50.0f * delta, 0.0f, character);
        } else if (key == sfKeyD) {
                character_move(50.0f * delta, 0.0f, character);
        }
}

void
character_update(struct character *character) {}
