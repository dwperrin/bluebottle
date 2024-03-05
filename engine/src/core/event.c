#include "event.h"

#include "core/bb_memory.h"

typedef struct register_event {
    void *listener;
    PFN_on_event callback;
} registered_event;

typedef struct event_code_entry {
    registered_event *events;
} event_code_entry;

// This should more than evenough codes...
#define MAX_MESSAGE_CODES 16384

// State structure.
typedef struct event_system_state {
    event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

/**
 * Event system internal state.
*/
static b8 is_initialised = FALSE;
static event_system_state state;

