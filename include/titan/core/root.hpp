// Copyright 2015 Bill Lin. All rights reserved.

#ifndef TITAN_CORE_ROOT_H_
#define TITAN_CORE_ROOT_H_

#include <windows.h>

#include "titan/core/titan.hpp"

void game_func(const HMODULE);
void game_load(const char *, struct game_api *);
void game_load_library(const HMODULE, struct game_api *);
void game_unload(struct game_api *);
void main_loop(void);
FILETIME last_write_time(const char *);

#endif  /* TITAN_CORE_ROOT_H_ */
