#pragma once

#include "defines.h"

/**
 * Appends the names of required extensions for this platfrom to
 * the names_darray, which should be created and passed in.
*/
void platform_get_required_extensions_names(const char*** names_darray);