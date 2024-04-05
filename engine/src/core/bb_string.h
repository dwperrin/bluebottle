#pragma once

#include "defines.h"

// Returns the length of the given string.
BBAPI u64 string_length(const char* str);

BBAPI char* string_duplicate(const char* str);

// Case-sensitive string comparision. True if the same, otherwise false
BBAPI b8 strings_equal(const char* str0, const char* str1);
