// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_CORE_ROOT_H_
#define TITAN_CORE_ROOT_H_

#include <windows.h>

#include "titan/core/titan.hpp"

struct game {
        HMODULE handle;
        FILETIME last_write_time;
        struct game_api api;
        struct game_state *state;
};

void game_create(char *, struct game *);
void game_destroy(struct game *);
void game_load(char *, struct game *);
void game_load_library(struct game *);
void game_unload(struct game *);
void main_loop(char *, struct game *);
FILETIME last_write_time(char *);

#endif  /* TITAN_CORE_ROOT_H_ */
