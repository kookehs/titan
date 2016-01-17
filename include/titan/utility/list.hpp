// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_UTILITY_LIST_HPP_
#define TITAN_UTILITY_LIST_HPP_

typedef void (*list_destroy_fn)(void *);
typedef void (*list_iterator)(void *);

struct list_node {
        void *data = nullptr;
        struct list_node *next = nullptr;
};

struct list {
        list_destroy_fn destroy;
        struct list_node *head = nullptr;
        size_t size = 0;
        size_t size_of_data = 0;
        struct list_node *tail = nullptr;
};

int list_create(size_t, list_destroy_fn, struct list **);
void list_destroy(struct list *);
void list_enumerate(list_iterator, struct list *);
int list_peek_back(void *, struct list *);
int list_peek_front(void *, struct list *);
int list_pop_front(void *, struct list *);
int list_push_back(void *, struct list *);
int list_push_front(void *, struct list *);

#endif /* TITAN_UTILITY_LIST_HPP_ */
