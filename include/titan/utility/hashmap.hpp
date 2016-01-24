// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_UTILITY_HASHMAP_HPP_
#define TITAN_UTILITY_HASHMAP_HPP_

using map_enum_fn = void (char *, void *);

struct bucket {
        size_t size;
        struct pair *pairs;
};

struct hashmap {
        size_t size;
        size_t size_of_data;
        struct bucket *buckets;
};

struct pair {
        char *key;
        void *value;
};

int hashmap_at(struct hashmap *, char *, void *);
int hashmap_create(size_t, size_t, struct hashmap **);
void hashmap_destroy(struct hashmap *);
int hashmap_enumerate(struct hashmap *, map_enum_fn *);
bool hashmap_exists(struct hashmap *, char *);
int hashmap_insert(char *, void *, struct hashmap *);
size_t hashmap_size(struct hashmap *);
struct pair *pair_at(struct bucket *, char *);
size_t string_hash(char *);

#endif /* TITAN_UTILITY_HASHMAP_HPP_ */
