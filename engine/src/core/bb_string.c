#include "core/bb_string.h"
#include "core/bb_memory.h"

#include <string.h>

u64 string_length(const char* str) {
    return strlen(str);
}

char* string_duplicate(const char* str) {
    u64 length = string_length(str);
    char* copy = bb_allocate(length + 1, MEMORY_TAG_STRING);
    bb_copy_memory(copy, str, length + 1);
    return copy;
}

// Case-sensitvie string compparison. True if the same, other false
b8 strings_equal(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}