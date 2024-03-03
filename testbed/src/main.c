#include <core/asserts.h>
#include <core/logger.h>

// TODO: Test
#include <platform/platform.h>

int main(void) {
    BBFATAL("A test message: %f", 3.14f);
    BBERROR("A test message: %f", 3.14f);
    BBWARN("A test message: %f", 3.14f);
    BBINFO("A test message: %f", 3.14f);
    BBDEBUG("A test message: %f", 3.14f);
    BBTRACE("A test message: %f", 3.14f);

    platform_state state;
    if (platform_startup(&state, "Bluebottle Engine Testbed", 100, 100, 1280, 720)) {
        while (TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);

    return 0;
}