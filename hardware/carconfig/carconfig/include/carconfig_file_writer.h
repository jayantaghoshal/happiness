#pragma once

#include "carconfig_base.h"

class CarConfigFileWriter
{
   public:
    static void writeCarConfig(const std::array<ccValue, cc_no_of_parameters>& allParams);
};

