/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <message_api.h>
#include <atomic>
#include <chrono>

enum class VersionHandshakeStatus { NotReceived, Ok, Error };

extern std::atomic<VersionHandshakeStatus> avmpVersionCheckOk;
extern std::atomic<std::chrono::steady_clock::time_point> lastAvmpHeartbeat;

// Use this function to initialize the vehicle signals manager inject handler
void vsm_inject_init(void);

// This function should be called when the lower layers has been setup and
// is ready for communication
void vsm_inject_start(void);

// This function is called to injected data or signal an error
void vsm_inject_inject(uint16_t signalId, void* message, bool injectError, const uint32_t length);
