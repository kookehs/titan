// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <string.h>

void
trim(char **input, char *delim) {
        size_t delim_length = strlen(delim);

        for (size_t i = 0; i < delim_length; ++i) {
                if ((*input)[0] == delim[i])
                        ++(*input);

                size_t input_length = strlen(*input);

                if ((*input)[input_length - 1] == delim[i])
                        (*input)[input_length - 1] = '\0';
        }
}
