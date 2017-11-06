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
