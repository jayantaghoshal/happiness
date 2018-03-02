/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace com {
namespace volvocars {
namespace installationmaster {

class InstallationMasterDaemon {
  private:
  public:
    InstallationMasterDaemon();
    ~InstallationMasterDaemon();

    void InitiatilizeBinder();
};

}  // namespace installationmaster
}  // namespace volvocars
}  // namespace com