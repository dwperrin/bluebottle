#include "game.h"

#include <core/logger.h>

b8 game_initalise(game *game_inst) {
    BBDEBUG("game_initialize() called");
    return TRUE;
}

b8 game_update(game *game_inst, f32 delta_time) {
    BBDEBUG("game_update() called");
    return TRUE;
}

b8 game_render(game *game_inst, f32 delta_time) {
    BBDEBUG("game_render() called");
    return TRUE;
}

void game_on_resize(game *game_inst, u32 width, u32 height) {
    BBDEBUG("game_on_resize() called");
}