// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <stdio.h>

#include "titan/utility/list.hpp"

void
print_node(void *data) {
        int value = *(int *)data;
        printf("%d, ", value);
}

int
main() {
        struct list *list = nullptr;
        list_create(sizeof(int), nullptr, &list);

        for (int i = 0; i < 10; ++i) {
                list_push_back(&i, list);
        }

        list_enumerate(print_node, list);

        int pop = 0;
        list_pop_front(&pop, list);
        list_enumerate(print_node, list);

        list_destroy(list);
}
