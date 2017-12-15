/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef LOCAL_CONFIG_H
#define LOCAL_CONFIG_H

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
#endif  // LOCAL_CONFIG_H
