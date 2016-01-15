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

struct rect_f;

enum side {
        none,
        bottom,
        left,
        right,
        top
};

bool physics_aabb_contains(float, float, struct rect_f *);
enum side physics_aabb_hit(struct rect_f *, struct rect_f *, struct rect_f *);
bool physics_aabb_intersects(struct rect_f *, struct rect_f *);

#endif /* TITAN_CORE_PHYSICS_HPP_ */
