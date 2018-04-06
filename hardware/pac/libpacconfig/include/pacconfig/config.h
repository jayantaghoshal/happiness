/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace pac {
namespace config {

// Helper class to configure PAC
class PacConfig {
  public:
    class Evs {
      public:
        // Checks if the EVS services are enabled in Android's system properties
        static bool IsEvsServicesEnabled();
    };
};

}  // namespace config
}  // namespace pac
