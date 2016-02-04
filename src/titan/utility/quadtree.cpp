// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/utility/quadtree.hpp"

#include <stdlib.h>

#include "titan/utility/hashmap.hpp"
#include "titan/utility/list.hpp"

int
quadtree_at(struct list *items, struct rect_f rect, struct quadtree tree) {
        if (items == nullptr)
                return 0;

        int quadrant = quadtree_quadrant(rect, tree);

        if (quadrant != -1 && tree.nodes != nullptr) {
                struct quadtree *node = nullptr;
                node = tree.nodes + sizeof(struct quadtree) * quadrant;
                quadtree_at(items, rect, *node);
        }

        list_copy(items, *tree.items);
        return 1;
}

int
quadtree_clear(struct quadtree *tree) {
        if (tree == nullptr)
                return 0;

        list_destroy(tree->items);
        struct quadtree *node = tree;

        for (size_t i = 0; i < tree->size; ++i, ++node) {
                if (node != nullptr) {
                        quadtree_clear(node);
                        node = nullptr;
                }
        }

        return 1;
}

int
quadtree_create(struct quadtree_info info, struct quadtree **tree) {
        if (tree == nullptr)
                return 0;

        if (*tree == nullptr)
                *tree = (struct quadtree *)malloc(sizeof(*(*tree)));

        if (*tree == nullptr)
                return 0;

        (*tree)->depth = info.depth;
        (*tree)->items = nullptr;
        (*tree)->list_info = info.list_info;
        (*tree)->max_depth = 5;
        (*tree)->max_items = 1;
        (*tree)->nodes = nullptr;
        (*tree)->rect = info.rect;
        (*tree)->size = 0;
        list_create(info.list_info, &(*tree)->items);
        return 1;
}

int
quadtree_insert(struct quadtree_item item, struct quadtree *tree) {
        if (tree == nullptr)
                return 0;

        if (tree->nodes != nullptr) {
                int quadrant = quadtree_quadrant(item.rect, *tree->nodes);

                if (quadrant != -1) {
                        struct quadtree *node = nullptr;
                        node = tree->nodes + sizeof(struct quadtree) * quadrant;
                        quadtree_insert(item, node);
                }

                return 0;
        }

        list_push_back(&item, tree->items);
        size_t *size = &tree->items->size;

        if (*size <= tree->max_items || tree->depth >= tree->max_depth)
                return 1;

        if (tree->nodes == nullptr)
                quadtree_split(tree);

        size_t i = 0;

        while (i < *size) {
                struct quadtree_item data;
                list_at(i, &data, *tree->items);
                int quadrant = quadtree_quadrant(data.rect, *tree);

                if (quadrant != -1) {
                        list_remove_at(i, nullptr, tree->items);
                        struct quadtree *node = nullptr;
                        node = tree->nodes + sizeof(struct quadtree) * quadrant;
                        quadtree_insert(data, node);
                } else {
                        ++i;
                }
        }

        return 1;
}

int
quadtree_quadrant(struct rect_f rect, struct quadtree tree) {
        int quadrant = -1;
        float origin_x = tree.rect.x + tree.rect.width / 2.0f;
        float origin_y = tree.rect.y + tree.rect.height / 2.0f;

        bool top_quadrant = (rect.y + rect.height < origin_y) ? true : false;
        bool bottom_quadrant = (rect.y > origin_y) ? true : false;
        bool left_quadrant = (rect.x + rect.width < origin_x);
        bool right_quadrant = (rect.x > origin_x);

        if (top_quadrant) {
                if (right_quadrant) {
                        quadrant = 0;
                } else if (left_quadrant) {
                        quadrant = 1;
                }
        } else if (bottom_quadrant) {
                if (left_quadrant) {
                        quadrant = 2;
                } else if (right_quadrant) {
                        quadrant = 3;
                }
        }

        return quadrant;
}

int
quadtree_split(struct quadtree *tree) {
        if (tree == nullptr)
                return 0;

        if (tree->nodes == nullptr) {
                size_t size = sizeof(struct quadtree) * 4;
                tree->nodes = (struct quadtree *)malloc(size);
        }

        if (tree->nodes == nullptr)
                return 0;

        tree->size = 4;

        float x = tree->rect.x;
        float y = tree->rect.y;
        float width = tree->rect.width / 2.0f;
        float height = tree->rect.height / 2.0f;
        size_t depth = tree->depth + 1;
        struct list_info info = tree->list_info;
        struct quadtree *nodes = tree->nodes;

        struct rect_f a;
        a.x = x + width;
        a.y = y;
        a.width = width;
        a.height = height;
        struct quadtree_info a_info;
        a_info.depth = depth;
        a_info.rect = a;
        a_info.list_info = info;
        struct quadtree *first_tree = nodes;
        quadtree_create(a_info, &first_tree);

        struct rect_f b;
        b.x = x;
        b.y = y;
        b.width = width;
        b.height = height;
        struct quadtree_info b_info;
        b_info.depth = depth;
        b_info.rect = b;
        b_info.list_info = info;
        struct quadtree *second_tree = nodes + sizeof(struct quadtree);
        quadtree_create(b_info, &second_tree);

        struct rect_f c;
        c.x = x;
        c.y = y + height;
        c.width = width;
        c.height = height;
        struct quadtree_info c_info;
        c_info.depth = depth;
        c_info.rect = c;
        c_info.list_info = info;
        struct quadtree *third_tree = nodes + sizeof(struct quadtree) * 2;
        quadtree_create(c_info, &third_tree);

        struct rect_f d;
        d.x = x + width;
        d.y = y + height;
        d.width = width;
        d.height = height;
        struct quadtree_info d_info;
        d_info.depth = depth;
        d_info.rect = d;
        d_info.list_info = info;
        struct quadtree *fourth_tree = nodes + sizeof(struct quadtree) * 3;
        quadtree_create(d_info, &fourth_tree);

        return 1;
}
