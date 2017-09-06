
#ifndef VSM_INJECT_H
#define VSM_INJECT_H

#include <message_api.h>

// Use this function to initialize the vehicle signals manager inject handler
void vsm_inject_init(void);

// This function should be called when the lower layers has been setup and
// is ready for communication
void vsm_inject_start(void);

// This function is called to injected data or signal an error
void vsm_inject_inject(uint16_t signalId, void *message, bool injectError, const uint32_t length);

#endif  // VSM_INJECT_H
