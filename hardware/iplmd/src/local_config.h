/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <string>

namespace Iplmd {
namespace LocalConfig {
void loadLocalConfig();

int getResourceGroups();

int getNofNodesInResourceGroup();

int getRequestMonitoringTmo();

int getNofLocalSoftwareComponents();
}
}
