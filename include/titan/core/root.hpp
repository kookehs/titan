// Copyright 2015 Bill Lin. All rights reserved.

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

struct game *game_create(const char *);
void game_destroy(struct game *);
void game_load(const char *, struct game *);
void game_load_library(struct game *);
void game_unload(struct game *);
void main_loop(const char *, struct game *);
FILETIME last_write_time(const char *);

#endif  /* TITAN_CORE_ROOT_H_ */
