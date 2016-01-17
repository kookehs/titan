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

void
quadtree_clear(struct quadtree *tree) {
        quadtree_rects_destroy(tree);

        struct quadtree *node = tree;

        for (size_t i = 0; i < tree->size; ++i, ++node) {
                if (node != nullptr) {
                        quadtree_clear(node);
                        node = nullptr;
                }
        }
}

int
quadtree_create(size_t depth, struct rect_f rect, struct quadtree *tree) {
        tree = (struct quadtree *)malloc(sizeof(*tree));

        if (tree == nullptr)
                return 0;

        tree->depth = depth;
        tree->rect = rect;
        tree->max_depth = 5;
        tree->max_rects = 10;
        tree->rects_size = 0;
        tree->size = 0;
        return 1;
}

void
quadtree_insert(struct rect_f rect, struct quadtree *tree) {
        if (tree->nodes != nullptr) {
                int quadrant = quadtree_quadrant(rect, tree->nodes);

                if (quadrant != -1)
                        quadtree_insert(rect, &tree->nodes[quadrant]);

                return;
        }

        quadtree_rects_insert(rect, tree);

        size_t size = tree->rects_size;

        if (size > tree->max_rects && tree->depth < tree->max_depth) {
                if (tree->nodes == nullptr)
                        quadtree_split(tree);

                size_t i = 0;

                while (i < size) {
                        int quadrant = quadtree_quadrant(tree->rects[i], tree);

                        if (quadrant != -1) {
                                struct rect_f object;
                                quadtree_rects_remove(i, tree->rects, &object);
                                quadtree_insert(object, &tree->nodes[quadrant]);
                        } else {
                                ++i;
                        }
                }
        }
}

int
quadtree_quadrant(struct rect_f rect, struct quadtree *tree) {
        int quadrant = -1;
        float origin_x = tree->rect.x + tree->rect.width / 2.0f;
        float origin_y = tree->rect.y + tree->rect.height / 2.0f;

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

void
quadtree_rects_destroy(struct quadtree *tree) {
        struct rect_f *rect = tree->rects;

        for (size_t i = 0; i < tree->rects_size; ++i, ++rect) {
                if (rect != nullptr) {
                        free(rect);
                        rect = nullptr;
                }
        }
}

int
quadtree_rects_insert(struct rect_f rect, struct quadtree *tree) {
        if (tree->rects_size == 0) {
                tree->rects = (struct rect_f *)malloc(sizeof(*tree->rects));

                if (tree->rects == nullptr)
                        return 0;

                tree->rects_size = 1;
        } else {
                size_t size = (tree->rects_size + 1) * sizeof(struct rect_f);
                struct rect_f *rects = tree->rects;
                struct rect_f *tmp = (struct rect_f *)realloc(rects, size);

                if (tmp == nullptr)
                        return 0;

                tree->rects = tmp;
                ++tree->rects_size;
        }

        tree->rects[tree->rects_size - 1] = rect;
        return 1;
}

void
quadtree_rects_remove(size_t index, struct rect_f *rects, struct rect_f *rect) {
        rects[index] = rects[index + 1];

        rect->x = rects[index].x;
        rect->y = rects[index].y;
        rect->width = rects[index].width;
        rect->height = rects[index].height;
}

void
quadtree_retrieve(struct rect_f rect, struct quadtree *tree, struct rect_f *objects) {
        int quadrant = quadtree_quadrant(rect, tree);

        if (quadrant != -1 && tree->nodes != nullptr)
                quadtree_retrieve(rect, tree, objects);

        if (objects == nullptr) {
                size_t size = tree->rects_size * sizeof(struct rect_f);
                objects = (struct rect_f *)malloc(size);
        } else {
                size_t size = tree->rects_size * sizeof(struct rect_f);
                struct rect_f *rects = objects;
                struct rect_f *tmp = (struct rect_f *)realloc(rects, size);
                objects = tmp;
        }
}

int
quadtree_split(struct quadtree *tree) {
        if (tree->size == 0) {
                tree->nodes = (struct quadtree *)malloc(sizeof(*tree->nodes));

                if (tree->nodes == nullptr)
                        return 0;

                tree->size = 1;
        } else {
                size_t size = (tree->size + 3) * sizeof(struct quadtree);
                struct quadtree *nodes = tree->nodes;
                struct quadtree *tmp = (struct quadtree *)realloc(nodes, size);

                if (tmp == nullptr)
                        return 0;

                tree->nodes = tmp;
                tree->size += 3;
        }

        float x = tree->rect.x;
        float y = tree->rect.y;
        float width = tree->rect.width / 2.0f;
        float height = tree->rect.height / 2.0f;

        struct rect_f a;
        a.x = x + width;
        a.y = y;
        a.width = width;
        a.height = height;
        quadtree_create(tree->depth + 1, a, &tree->nodes[tree->size - 4]);

        struct rect_f b;
        b.x = x;
        b.y = y;
        b.width = width;
        b.height = height;
        quadtree_create(tree->depth + 1, b, &tree->nodes[tree->size - 3]);

        struct rect_f c;
        c.x = x;
        c.y = y + height;
        c.width = width;
        c.height = height;
        quadtree_create(tree->depth + 1, c, &tree->nodes[tree->size - 2]);

        struct rect_f d;
        d.x = x + width;
        d.y = y + height;
        d.width = width;
        d.height = height;
        quadtree_create(tree->depth + 1, d, &tree->nodes[tree->size - 1]);

        return 1;
}
