// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <stdio.h>

#include "titan/utility/hashmap.hpp"

void
print_pair(char *key, void *value) {
        printf("%s : %s\n", key, (char *)value);
}

int
main() {
        constexpr size_t size = sizeof(char) * 255;
        struct hashmap *map;
        hashmap_create(size, 8, &map);
        char buffer[size];

        hashmap_insert("fruit", "apple", map);
        hashmap_insert("color", "red", map);
        hashmap_insert("letter", "a", map);
        hashmap_insert("number", "3", map);
        hashmap_insert("color", "blue", map);
        hashmap_insert("os", "windows", map);

        printf("Hashmap:\n");
        hashmap_enumerate(print_pair, map);

        hashmap_at("letter", buffer, map);
        printf("letter : %s\n", buffer);

        printf("Exists: %d", hashmap_exists("fruit", map));

        hashmap_destroy(map);

        int x = 0;
        scanf("%d", &x);
}
