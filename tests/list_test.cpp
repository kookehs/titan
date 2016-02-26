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
        printf("%d ", value);
}

int
main() {
        struct list_info info;
        info.destroy = nullptr;
        info.size_of_data = sizeof(int);
        struct list *list = nullptr;
        list_create(info, &list);

        for (int i = 0; i < 10; ++i) {
                list_push_back(&i, list);
        }

        printf("Intial list: ");
        list_enumerate(print_node, *list);

        printf("\nList at index 2: ");
        int at;
        list_at(2, &at, *list);
        printf("%d", at);

        printf("\nPeek back: ");
        int back;
        list_peek_back(&back, *list);
        printf("%d", back);

        printf("\nPeek front: ");
        int front;
        list_peek_front(&front, *list);
        printf("%d", front);

        printf("\nPop back: ");
        int pop_back;
        list_pop_back(&pop_back, list);
        printf("%d", pop_back);

        printf("\nPeek back: ");
        int back_after_pop;
        list_peek_back(&back_after_pop, *list);
        printf("%d", back_after_pop);

        printf("\nPop front: ");
        int pop_front;
        list_pop_front(&pop_front, list);
        printf("%d", pop_front);

        printf("\nPeek front: ");
        int front_after_pop;
        list_peek_front(&front_after_pop, *list);
        printf("%d", front_after_pop);

        printf("\nPush front: ");
        int push_front = 99;
        list_push_front(&push_front, list);
        printf("%d", push_front);

        printf("\nList: ");
        list_enumerate(print_node, *list);

        printf("\nPush back: ");
        int push_back = 100;
        list_push_back(&push_back, list);
        printf("%d", push_back);

        printf("\nList: ");
        list_enumerate(print_node, *list);

        printf("\nRemove at: ");
        int remove_at;
        list_remove_at(list->size / 2, &remove_at, list);
        printf("%d", remove_at);

        printf("\nList: ");
        list_enumerate(print_node, *list);

        list_destroy(list);
}
