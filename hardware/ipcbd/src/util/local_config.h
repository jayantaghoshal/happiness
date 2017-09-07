/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef LOCAL_CONFIG_H
#define LOCAL_CONFIG_H

#include <string>

namespace ipcb
{
namespace Utils
{
void loadLocalConfig();

int getResourceGroups();

int getNofNodesInResourceGroup();

int getRequestMonitoringTmo();

int getNofLocalSoftwareComponents();
}
}
#endif  // LOCAL_CONFIG_H
