/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "carconfig_base.h"

class CarConfigFileWriter {
  public:
    static void writeCarConfig(
            const std::array<Carconfig_base::ccValue, Carconfig_base::cc_no_of_parameters>& allParams);
};
