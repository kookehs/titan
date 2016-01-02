// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#define _CRTDBG_MAP_ALLOC

#include "titan/core/root.hpp"

#include <stdlib.h>
#include <crtdbg.h>

int
main() {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        char *dll_path = "titan.dll";
        struct game game;
        game_create(dll_path, &game);
        main_loop(dll_path, &game);
        game_destroy(&game);
        _CrtDumpMemoryLeaks();
        return 0;
}

inline void
game_create(char *dll_path, struct game *game) {
        game_load(dll_path, game);
        game->api.init(&game->state);
}

inline void
game_destroy(struct game *game) {
        game_unload(game);
}

inline void
game_load(char *dll_path, struct game *game) {
        char* dll_temp_path = "titan_temp.dll";
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
last_write_time(char *dll_path) {
        FILETIME write_time;
        WIN32_FILE_ATTRIBUTE_DATA data;

        if (GetFileAttributesEx(dll_path, GetFileExInfoStandard, &data))
                write_time = data.ftLastWriteTime;

        return write_time;
}

void
main_loop(char *dll_path, struct game *game) {
        while (game->api.loop(&game->state) == 0) {
                FILETIME write_time = last_write_time(dll_path);

                if (CompareFileTime(&write_time, &game->last_write_time) != 0) {
                        game_unload(game);
                        game_load(dll_path, game);
                }
        }
}
