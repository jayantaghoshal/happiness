/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "ivi-logging.h"

/// We define our actual LogContext type, which defines the list of backends to use for the logging. Here, we simply
/// reuse the default list of backends
typedef logging::DefaultLogContext LogContext;
