// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_UTILITY_QUADTREE_HPP_
#define TITAN_UTILITY_QUADTREE_HPP_

struct quadtree {
        size_t max_depth;
        size_t max_objects;
        size_t level;
        float x;
        float y;
        float width;
        float height;
        struct quadtree *nodes;
};

int quadtree_create(size_t, float, float, float, float, struct quadtree *);

#endif /* TITAN_UTILITY_QUADTREE_HPP_ */
