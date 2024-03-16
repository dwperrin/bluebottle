#include "application.h"
#include "games_types.h"

#include "logger.h"

#include "platform/platform.h"
#include "core/bb_memory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

#include "renderer/renderer_frontend.h"

typedef struct application_state {
    game *game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    clock clock;
    f64 last_time;
} application_state;

static b8 initialised = FALSE;
static application_state app_state;

// Event Handlers
b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);

b8 application_create(game *game_inst) {
    if (initialised) {
        BBERROR("application_create called more than once.");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // Initalise subsystems
    initialise_logging();
    input_initialise();

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

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    if (!platform_startup(
            &app_state.platform,
            game_inst->app_config.name,
            game_inst->app_config.start_pos_x,
            game_inst->app_config.start_pos_y,
            game_inst->app_config.start_width,
            game_inst->app_config.start_height)) {
        return FALSE;
    }

    // Renderer startup
    if (!renderer_initialise(game_inst->app_config.name, &app_state.platform)) {
        BBFATAL("Failed to initialise renderer. Aborting application.");
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
    clock_start(&app_state.clock);
    clock_update(&app_state.clock);
    app_state.last_time = app_state.clock.elapsed;
    f64 running_time = 0;
    u8 frame_count = 0;
    f64 target_frame_seconds = 1.0f / 60;

    BBINFO(get_memory_usage_str());

    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            //  Update clock and get delta time.
            clock_update(&app_state.clock);
            f64 current_time = app_state.clock.elapsed;
            f64 delta = (current_time - app_state.last_time);
            f64 frame_start_time = platform_get_absolute_time();

            if (!app_state.game_inst->update(app_state.game_inst, (f32)delta)) {
                BBFATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            // Call the game's render routine
            if (!app_state.game_inst->render(app_state.game_inst, (f32)delta)) {
                BBFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            // TODO: refactor packet creation
            render_packet packet;
            packet.delta_time = delta;
            renderer_draw_frame(&packet);

            // Figure out how long the frame took and, if below
            f64 frame_end_time = platform_get_absolute_time();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0) {
                u64 remaining_ms = (remaining_seconds * 1000);

                // If there is time left, give it back to the OS.
                b8 limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;

            }

            // NOTE: Input update/state copying should always be handled
            // after any input should be recorded; I.E. before this line.
            // As a safety, input is the last thing to be updated before
            // this frame ends.
            input_update(delta);

            // Update last time
            app_state.last_time = current_time;
        }
    }

    app_state.is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);
    event_shutdown();
    input_shutdown();

    rendered_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;

}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            BBINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
            app_state.is_running = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_ESCAPE) {
            // NOTE: Technically firing an event to itself, but there may be other listeners.
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0,  data);

            // Block anaything esle from processing this.
            return TRUE;
        } else if (key_code == KEY_A) {
            // Example on checking for a key
            BBDEBUG("Explicit - A key pressed!");
        } else {
            BBDEBUG("'%c' key pressed in window", key_code);
        }
    } else if (code == EVENT_CODE_KEY_RELEASED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_B) {
            // Example on checking for a key
            BBDEBUG("Explicit - B key released!");
        } else {
            BBDEBUG("'%c' key released in window.", key_code);
        }
    }
    return FALSE;
}