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

#include "titan/utility/list.hpp"
#include "titan/utility/types.hpp"

struct quadtree {
        size_t depth = 0;
        struct list *items = nullptr;
        struct list_info list_info;
        size_t max_depth = 5;
        size_t max_items = 1;
        struct quadtree *nodes = nullptr;
        struct rect_f rect;
        size_t size = 0;
};

struct quadtree_info {
        size_t depth = 0;
        struct list_info list_info;
        struct rect_f rect;
};

struct quadtree_item {
        void *data;
        struct rect_f rect;
};

int quadtree_at(struct list *, struct rect_f, struct quadtree);
int quadtree_clear(struct quadtree *);
int quadtree_create(struct quadtree_info, struct quadtree **);
int quadtree_insert(struct quadtree_item, struct quadtree *);
int quadtree_quadrant(struct rect_f, struct quadtree);
int quadtree_split(struct quadtree *);

#endif /* TITAN_UTILITY_QUADTREE_HPP_ */
