// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "titan/utility/misc.hpp"

int
main() {
        char *leading_space = " The leading space should be trimmed.";
        trim(&leading_space, " ");
        printf("%s\n", leading_space);

        char *ending_newline = (char *)malloc(256);
        strcpy(ending_newline, "The newline at the end should be trimmed.\n");
        trim(&ending_newline, "\n");
        printf("%s\n", ending_newline);
        free(ending_newline);
}
