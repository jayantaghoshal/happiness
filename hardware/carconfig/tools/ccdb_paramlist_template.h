#pragma once

#include <map>
#include <vector>

namespace CarConfigParamList
{
    /* Do not edit by hand!
    The values below is extracted from the file ##FILENAME##
    on ##DATE## with the tool ##TOOL##
    */
    const std::map<int,std::vector<int>> paramList =
    {
        {% for param in paramlist %}
        {
            {{param.number}}, //{{param.name}}
            {
                {% for value in param.values %}
                0x{{value.value}},//{{value.desc}}
                {% endfor %}
            }
        },
        {% endfor %}
    };
}
