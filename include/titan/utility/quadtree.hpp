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

#include "titan/utility/types.hpp"

struct quadtree {
        size_t depth = 0;
        size_t max_depth = 5;
        size_t max_rects = 10;
        struct quadtree *nodes = nullptr;
        struct rect_f rect;
        struct rect_f *rects = nullptr;
        size_t rects_size = 0;
        size_t size = 0;
};

void quadtree_clear(struct quadtree *);
int quadtree_create(size_t, struct rect_f, struct quadtree *);
void quadtree_insert(struct rect_f, struct quadtree *);
int quadtree_quadrant(struct rect_f, struct quadtree *);
int quadtree_split(struct quadtree *);

void quadtree_rects_destroy(struct quadtree *);
int quadtree_rects_insert(struct rect_f, struct quadtree *);
void quadtree_rects_remove(size_t, struct rect_f *, struct rect_f *);

#endif /* TITAN_UTILITY_QUADTREE_HPP_ */
