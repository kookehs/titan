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
print_pair(char *key, char *value) {
        printf("%s : %s\n", key, value);
}

int
main() {
        struct map *map = map_create(8);
        char buffer[255];

        hashmap_insert("fruit", "apple", map);
        hashmap_insert("color", "red", map);
        hashmap_insert("letter", "a", map);
        hashmap_insert("number", "3", map);
        hashmap_insert("color", "blue", map);
        hashmap_insert("os", "windows", map);

        hashmap_at(map, "letter", sizeof(buffer), buffer);
        printf("letter : %s\n", buffer);

        hashmap_enumerate(map, print_pair);

        hashmap_destroy(map);

        int x = 0;
        scanf("%d", &x);
}
