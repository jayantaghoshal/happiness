/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "local_config.h"

int g_nrOfResourceGroups = 0;
int g_nrOfNodesInResourceGroup = 0;
int g_requestMonitoringTmo = 0;
int g_nrOfLocalSoftwareComponents = 0;

namespace InfotainmentIpService
{
namespace Utils
{
void loadLocalConfig()
{
}

int getResourceGroups()
{
    return g_nrOfResourceGroups;
}

int getNofNodesInResourceGroup()
{
    return g_nrOfNodesInResourceGroup;
}

int getRequestMonitoringTmo()
{
    return g_requestMonitoringTmo;
}

int getNofLocalSoftwareComponents()
{
    return g_nrOfLocalSoftwareComponents;
}
}
}
