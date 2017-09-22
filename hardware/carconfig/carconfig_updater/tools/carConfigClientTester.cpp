#include <cstdint>
#include <unistd.h>
#include "carconfig.h"
#include <cc_parameterlist_map.h>

using namespace CarConfigParamList;

int32_t main(int argc, char *argv[])
{
    // Print all carconfigs relevant to IHU
    for(const auto &param :paramList)
    {
        auto my_param = carconfig::getValue(param.first);
        printf("CC%d: %d \n",param.first, my_param);
    }
}
