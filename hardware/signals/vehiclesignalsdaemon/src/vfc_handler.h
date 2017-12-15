/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <ECD_dataelement.h>

/*
 * To limit the number of activatevfc messages sent to the VIP over uart1 we
 * here
 * handle a rate limiter.
 *
 * If this function returns true then the vfc shall be sent to the VIP
 * WITH the data provided in the vfc argument.
 *
 * If this function returns false the vfc shall not be sent to the VIP at all.
 */
bool shallSendVfcToVIP(const ActivateVfc &vfc);
