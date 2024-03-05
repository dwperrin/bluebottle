#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "games_types.h"

// Externally-defined function to create a game.
extern b8 create_game(game *out_game);

/**
 * The main entry point of the application.
 */

int main(void) {
    // Request the game instance from the appication.
    game game_inst;
    if (!create_game(&game_inst)) {
        BBFATAL("Could not create game!");
        return -1;
    }

    // Ensure the function pointers exist.
    if (!game_inst.render || !game_inst.update || !game_inst.initialise || !game_inst.on_resize) {
        BBFATAL("The game's function pointers must be assigned!");
        return -2;
    }

    // Initilisation
    if (!application_create(&game_inst)) {
        BBINFO("Application failed to create!");
        return 1;
    }

    // Game loop
    if (!application_run()) {
        BBINFO("Application did not shutdown gracefully.");
        return 2;
    }

    return 0;
}