// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_PHYSICS_HPP_
#define TITAN_CORE_PHYSICS_HPP_

enum side {
        none,
        bottom,
        left,
        right,
        top
};

struct aabb {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
};

bool physics_aabb(struct aabb *, struct aabb *);
enum side physics_aabb_hit_side(struct aabb *, struct aabb *, struct aabb *);

#endif /* TITAN_CORE_PHYSICS_HPP_ */
