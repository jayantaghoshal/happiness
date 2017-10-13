#pragma once

#include "carconfig_base.h"

class CarConfigFileWriter
{
   public:
    static void writeCarConfig(const std::array<Carconfig_base::ccValue, Carconfig_base::cc_no_of_parameters>& allParams);
};

