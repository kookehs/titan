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
