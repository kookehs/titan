// Copyright 2015 Bill Lin. All rights reserved.

#include "titan/core/root.hpp"

#include <stdlib.h>

int
main() {
        const char *dll_path = "titan.dll";
        struct game *game = game_create(dll_path);
        main_loop(dll_path, game);
        game_destroy(game);

        return 0;
}

inline struct game *
game_create(const char *dll_path) {
        struct game *game = (struct game *)malloc(sizeof(*game));
        game->state = (struct game_state *)malloc(sizeof(*game->state));
        game_load(dll_path, game);
        game->api.init(game->state);

        return game;
}

inline void
game_destroy(struct game *game) {
        game_unload(game);
        free(game->state);
        free(game);
}

inline void
game_load(const char *dll_path, struct game *game) {
        const char* dll_temp_path = "titan_temp.dll";
        CopyFile(dll_path, dll_temp_path, false);
        game->handle = LoadLibrary(dll_temp_path);

        if (game->handle) {
                game->last_write_time = last_write_time(dll_path);
                game_load_library(game);
        }
}

inline void
game_load_library(struct game *game) {
        game->api = *(struct game_api *)GetProcAddress(game->handle, "api");
}

inline void
game_unload(struct game *game) {
        if (game->handle) {
                FreeLibrary(game->handle);
                game->handle = nullptr;
        }
}

inline FILETIME
last_write_time(const char *dll_path) {
        FILETIME write_time;
        WIN32_FILE_ATTRIBUTE_DATA data;

        if (GetFileAttributesEx(dll_path, GetFileExInfoStandard, &data))
                write_time = data.ftLastWriteTime;

        return write_time;
}

void
main_loop(const char *dll_path, struct game *game) {
        while (game->api.loop(game->state) == false) {
                FILETIME write_time = last_write_time(dll_path);

                if (CompareFileTime(&write_time, &game->last_write_time) != 0) {
                        game_unload(game);
                        game_load(dll_path, game);
                }
        }
}
