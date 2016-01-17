// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "titan/utility/hashmap.hpp"

#include <stdlib.h>
#include <string.h>

int
hashmap_at(struct hashmap *map, char *key, size_t size, char *out) {
        if (map == nullptr || key == nullptr)
                return 0;

        size_t index = string_hash(key) % map->size;
        struct bucket *bucket = &map->buckets[index];
        struct pair *pair = pair_at(bucket, key);

        if (pair == nullptr)
                return 0;

        if (out == nullptr && size == 0)
                return strlen(pair->value) + 1;

        if (out == nullptr)
                return 0;

        size_t length = strlen(pair->value);

        if (length >= size)
                return 0;

        strncpy(out, pair->value, strlen(pair->value) + 1);
        return 1;
}

int
hashmap_create(size_t capacity, struct hashmap **map) {
        if (capacity < 1)
                return 0;

        *map = (struct hashmap *)malloc(sizeof(*(*map)));

        if (*map == nullptr)
                return 0;

        (*map)->size = capacity;
        size_t size = (*map)->size * sizeof(*(*map)->buckets);
        (*map)->buckets = (struct bucket *)malloc(size);

        if ((*map)->buckets == nullptr) {
                free(*map);
                *map = nullptr;
        } else {
               memset((*map)->buckets, 0, size);
        }

        return 1;
}

void
hashmap_destroy(struct hashmap *map) {
        if (map == nullptr)
                return;

        struct bucket *bucket = map->buckets;

        for (size_t i = 0; i < map->size; ++i, ++bucket) {
                struct pair *pair = bucket->pairs;

                for (size_t j = 0; j < bucket->size; ++j, ++pair) {
                        free(pair->key);
                        pair->key = nullptr;
                        free(pair->value);
                        pair->value = nullptr;
                }

                free(bucket->pairs);
                bucket->pairs = nullptr;
        }

        free(map->buckets);
        map->buckets = nullptr;
        free(map);
        map = nullptr;
}

int
hashmap_enumerate(struct hashmap *map, map_enum_fn *func) {
        if (map == nullptr || func == nullptr)
                return 0;

        struct bucket *bucket = map->buckets;

        for (size_t i = 0; i < map->size; ++i, ++bucket) {
                struct pair *pair = bucket->pairs;

                for (size_t j = 0; j < bucket->size; ++j, ++pair) {
                        func(pair->key, pair->value);
                }
        }

        return 1;
}

bool
hashmap_exists(struct hashmap *map, char *key) {
        if (map == nullptr || key == nullptr)
                return false;

        size_t index = string_hash(key) % map->size;
        struct bucket *bucket = &map->buckets[index];
        struct pair *pair = pair_at(bucket, key);

        if (pair == nullptr)
                return false;

        return true;
}

int
hashmap_insert(char *key, char *value, struct hashmap *map) {
        if (map == nullptr || key == nullptr || value == nullptr)
                return 0;

        size_t key_length = strlen(key) + 1;
        size_t value_length = strlen(value) + 1;
        size_t index = string_hash(key) % map->size;
        struct bucket *bucket = &map->buckets[index];
        struct pair *pair = pair_at(bucket, key);

        if (pair) {
                if (strlen(pair->value) < value_length) {
                        size_t size = value_length * sizeof(char);
                        char *tmp = (char *)realloc(pair->value, size);

                        if (tmp == nullptr)
                                return 0;

                        pair->value = tmp;
                }

                strncpy(pair->value, value, strlen(value) + 1);
                return 1;
        }

        char *new_key = (char *)malloc(key_length * sizeof(char));

        if (new_key == nullptr)
                return 0;

        char *new_value = (char *)malloc(value_length * sizeof(char));

        if (new_value == nullptr) {
                free(new_key);
                new_key = nullptr;
                return 0;
        }

        if (bucket->size == 0) {
                bucket->pairs = (struct pair *)malloc(sizeof(*bucket->pairs));

                if (bucket->pairs == nullptr) {
                        free(new_key);
                        new_key = nullptr;
                        free(new_value);
                        new_value = nullptr;
                        return 0;
                }

                bucket->size = 1;
        } else {
                size_t size = (bucket->size + 1) * sizeof(struct pair);
                struct pair *tmp = (struct pair *)realloc(bucket->pairs, size);

                if (tmp == nullptr) {
                        free(new_key);
                        free(new_value);
                        return 0;
                }

                bucket->pairs = tmp;
                ++bucket->size;
        }

        pair = &bucket->pairs[bucket->size - 1];
        pair->key = new_key;
        pair->value = new_value;
        strncpy(pair->key, key, key_length);
        strncpy(pair->value, value, value_length);
        return 1;
}

size_t
hashmap_size(struct hashmap *map) {
        if (map == nullptr)
                return 0;

        size_t count = 0;
        struct bucket *bucket = map->buckets;

        for (size_t i = 0; i < map->size; ++i, ++bucket) {
                struct pair *pair = bucket->pairs;

                for (size_t j = 0; j < bucket->size; ++j, ++pair) {
                        ++count;
                }
        }

        return count;
}

struct pair *
pair_at(struct bucket *bucket, char *key) {
        if (bucket == nullptr || bucket->size == 0)
                return nullptr;

        struct pair *pair = bucket->pairs;

        for (size_t i = 0; i < bucket->size; ++i, ++pair) {
                if (pair->key && pair->value) {
                        if (strcmp(pair->key, key) == 0)
                                return pair;
                }
        }

        return nullptr;
}

size_t
string_hash(char *input) {
        size_t hash = 42;
        char c = 0;

        while (c = *input++)
                hash = (hash << 5) + hash + c;

        return hash;
}
