#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if BBRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} log_level;

b8 initialise_logging();
void shutdown_logging();

BBAPI void log_output(log_level level, const char *message, ...);

// Logs a fatal level message
#define BBFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#ifndef BBERROR
// Logs an error level message
#define BBERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if LOG_WARN_ENABLED
// Logs a warning level message
#define BBWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_WARN-ENABLED !=1
#define BBWARN(message, ...)
#endif

#if LOG_INFO_ENABLED
// Logs a warning level message
#define BBINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_WARN-ENABLED !=1
#define BBINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED
// Logs a warning level message
#define BBDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_WARN-ENABLED !=1
#define BBDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED
// Logs a warning level message
#define BBTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_WARN-ENABLED !=1
#define BBTRACE(message, ...)
#endif
