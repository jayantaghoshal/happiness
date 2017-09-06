/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef STUB_LOCAL_CONFIG_H
#define STUB_LOCAL_CONFIG_H

extern int g_nrOfResourceGroups;
extern int g_nrOfNodesInResourceGroup;
extern int g_requestMonitoringTmo;
extern int g_nrOfLocalSoftwareComponents;

namespace InfotainmentIpService
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
#endif  // STUB_LOCAL_CONFIG_H
