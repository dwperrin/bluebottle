#pragma once

#include "defines.h"

// Disable assertions by commentin out the below line
#define BBASSERTIONS_ENABLED

#ifdef BBASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

BBAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define BBASSERT(expr)                                               \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define BBASSERT_MSG(expr, message)                                       \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define BBASSERT_DEBUG(expr)                                         \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#else
#define BBASSERT_DEBUG(expr)  // Does nothing at all
#endif

#else
#define BBASSERT(expr)        // Does nothing at all
#define BBASSERT_MSG(expr)    // Does nothing at all
#define BBASSERT_DEBUG(expr)  // Does nothing at all
#endif