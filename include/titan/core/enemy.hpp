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

struct sfRenderWindow;
struct sfSprite;
struct sfTexture;

struct enemy {
        float x = 0.0f;
        float y = 0.0f;
        float dx = 0.0f;
        float dy = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        sfSprite *sprite = nullptr;
        sfTexture *texture = nullptr;
};

struct enemy_list {
        size_t size = 0;
        struct enemy *enemys;
};

int enemy_create(char *, float, float, struct enemy *);
void enemy_destroy(struct enemy *);
void enemy_move(float, struct enemy *);
void enemy_update(float, struct enemy *);

int enemy_list_add(char *, float, float, struct enemy_list *);
void enemy_list_destroy(struct enemy_list *);
void enemy_list_draw(struct enemy_list *, sfRenderWindow *);

#endif /* TITAN_CORE_ENEMY_HPP_ */
