#include "application.h"

#include "games_types.h"
#include "logger.h"
#include "core/bb_memory.h"
#include "platform/platform.h"
#include "core/event.h"

typedef struct application_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialised = FALSE;
static application_state app_state;

b8 application_create(game *game_inst) {
    if (initialised) {
        BBERROR("application_create called more than once.");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // Initalise subsystems
    initialise_logging();

    // TODO: Remove this
    BBFATAL("A test message: %f", 3.14f);
    BBERROR("A test message: %f", 3.14f);
    BBWARN("A test message: %f", 3.14f);
    BBINFO("A test message: %f", 3.14f);
    BBDEBUG("A test message: %f", 3.14f);
    BBTRACE("A test message: %f", 3.14f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!event_initialise()) {
        BBERROR("Event system failed initalisation. Application cannot continue.");
        return FALSE;
    }

    if (!platform_startup(
            &app_state.platform,
            game_inst->app_config.name,
            game_inst->app_config.start_pos_x,
            game_inst->app_config.start_pos_y,
            game_inst->app_config.start_width,
            game_inst->app_config.start_height)) {
        return FALSE;
    }

    // Initialise the game.
    if (!app_state.game_inst->initialise(app_state.game_inst)) {
        BBFATAL("Game failed to initialise");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialised = TRUE;

    return TRUE;
}

b8 application_run() {
    BBINFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0)) {
                BBFATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (f32)0)) {
                BBFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;

    event_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;
}