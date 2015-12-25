// Copyright 2015 Bill Lin. All rights reserved.

#include "titan/core/root.hpp"

int main() {
        main_loop();

        return 0;
}

inline void
game_destroy(struct game_api *game) {
        game_unload(game);
        free(game);
}

inline FARPROC
game_func(const HMODULE handle, const char* name) {
    return GetProcAddress(handle, name);
}

void
game_load(const char *dll_path, struct game_api *game) {
        const char* dll_temp_path = "titan_temp.dll";
        CopyFile(dll_path, dll_temp_path, false);
        game->handle = LoadLibrary(dll_temp_path);

        if (game->handle) {
                game->last_write_time = last_write_time(dll_path);
                game_load_library(game->handle, game);
        }
}

inline void
game_load_library(const HMODULE handle, struct game_api *game) {
        game->game_init = (game_init_ptr)game_func(handle, "game_init");
        game->game_update = (game_update_ptr)game_func(handle, "game_update");
}

void
game_unload(struct game_api *game) {
        if (game->handle) {
                FreeLibrary(game->handle);
                game->handle = nullptr;
                game->game_init = nullptr;
                game->game_update = nullptr;
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
main_loop(void) {
        const char *dll_path = "titan.dll";
        struct game_api *game = (struct game_api *)malloc(sizeof(*game));
        game_load(dll_path, game);
        game->state = game->game_init();

        while (game->game_update(game->state) == false) {
                FILETIME write_time = last_write_time(dll_path);

                if (CompareFileTime(&write_time, &game->last_write_time) != 0) {
                        game_unload(game);
                        game_load(dll_path, game);
                }
        }

        game_destroy(game);
}
