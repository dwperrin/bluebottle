#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    BBFATAL("A test message: %f", 3.14f);
    BBERROR("A test message: %f", 3.14f);
    BBWARN("A test message: %f", 3.14f);
    BBINFO("A test message: %f", 3.14f);
    BBDEBUG("A test message: %f", 3.14f);
    BBTRACE("A test message: %f", 3.14f);

    BBASSERT(1 == 0);

    return 0;
}