// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/utility/physics.hpp"

bool
physics_aabb(struct aabb *a, struct aabb *b) {
        if ((a->x > b->x + b->width) || (a->x + a->width < b->x) ||
            (a->y > b->y + b->height) || (a->y + a->height < b->y)) {
                return false;
        }

        return true;
}

enum side
physics_aabb_hit_side(struct aabb *a, struct aabb *b, struct aabb *c) {
        enum side hit = none;

        if ((c->y > b->y + b->height) && (a->y < b->y + b->height)) {
                hit = bottom;
        } else if ((c->x + c->width < b->x) && (a->x + a->width > b->x)) {
                hit = left;
        } else if ((c->x > b->x + b->width) && (a->x < b->x + b->width)) {
                hit = right;
        } else if ((c->y + c->height < b->y) && (a->y + a->height > b->y)) {
                hit = top;
        }

        return hit;
}
