// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/utility/list.hpp"

#include <stdlib.h>
#include <string.h>

int
list_at(size_t index, void *out, struct list *list) {
        if (out == nullptr || list == nullptr)
                return 0;

        if (list->size <= index)
                return 0;

        struct list_node *current = list->head;

        for (size_t i = 0; i < index; ++i, current = current->next) {
                current = current->next;
        }

        memcpy(out, current->data, list->size_of_data);
        return 1;
}

int
list_copy(struct list *out, struct list *list) {
        if (out == nullptr || list == nullptr)
                return 0;

        struct list_node *current = list->head;

        while (current != nullptr) {
                list_push_front(current, out);
                current = current->next;
        }

        return 1;
}

int
list_create(struct list_info *info, struct list **list) {
        if (info->size_of_data < 0)
                return 0;

        *list = (struct list *)malloc(sizeof(*(*list)));
        (*list)->destroy = info->destroy;
        (*list)->head = nullptr;
        (*list)->size = 0;
        (*list)->size_of_data = info->size_of_data;
        (*list)->tail = nullptr;
        return 1;
}

void
list_destroy(struct list *list) {
        if (list == nullptr)
                return;

        struct list_node *current = nullptr;

        while (list->head != nullptr) {
                current = list->head;
                list->head = current->next;

                if (list->destroy != nullptr)
                        list->destroy(current->data);

                free(current->data);
                current->data = nullptr;
                free(current);
                current = nullptr;
        }
}

void
list_enumerate(list_iterator iterator, struct list *list) {
        if (iterator == nullptr || list == nullptr)
                return;

        struct list_node *current = list->head;

        while (current != nullptr) {
                iterator(current->data);
                current = current->next;
        }
}

int
list_peek_back(void *out, struct list *list) {
        if (out == nullptr || list == nullptr)
                return 0;

        memcpy(out, list->tail->data, list->size_of_data);
        return 1;
}

int
list_peek_front(void *out, struct list *list) {
        if (out == nullptr || list == nullptr)
                return 0;

        memcpy(out, list->head->data, list->size_of_data);
        return 1;
}

int
list_pop_front(void *out, struct list *list) {
        if (out == nullptr || list == nullptr)
                return 0;

        memcpy(out, list->head->data, list->size_of_data);
        struct list_node *tmp = list->head;
        list->head = list->head->next;
        --list->size;

        if (list->destroy)
                list->destroy(tmp->data);

        free(tmp->data);
        tmp->data = nullptr;
        free(tmp);
        tmp = nullptr;
        return 1;
}

int
list_push_back(void *data, struct list *list) {
        if (data == nullptr || list == nullptr)
                return 0;

        struct list_node *tmp = (struct list_node *)malloc(sizeof(*tmp));

        if (tmp == nullptr)
                return 0;

        tmp->data = malloc(list->size_of_data);
        tmp->next = nullptr;
        memcpy(tmp->data, data, list->size_of_data);

        if (list->size == 0) {
                list->head = tmp;
                list->tail = tmp;
        } else {
                list->tail->next = tmp;
                list->tail = tmp;
        }

        ++list->size;
        return 1;
}

int
list_push_front(void *data, struct list *list) {
        if (data == nullptr || list == nullptr)
                return 0;

        struct list_node *tmp = (struct list_node *)malloc(sizeof(*tmp));

        if (tmp == nullptr)
                return 0;

        tmp->data = malloc(list->size_of_data);
        tmp->next = list->head;
        memcpy(tmp->data, data, list->size_of_data);

        list->head = tmp;

        if (list->size == 0)
                list->tail = tmp;

        ++list->size;
        return 1;
}

int
list_remove_at(size_t index, void *out, struct list *list) {
        if (list == nullptr)
                return 0;

        if (list->size <= index)
                return 0;

        struct list_node *current = list->head;

        for (size_t i = 0; i < index; ++i, current = current->next) {
                current = current->next;
        }

        if (out != nullptr)
                memcpy(out, current->data, list->size_of_data);

        struct list_node *next = current->next;
        current->data = next->data;
        current->next = next->next;
        return 1;
}
