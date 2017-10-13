#include "carconfig_file_writer.h"
#include "carconfig_base.h"
#include <stdexcept>
#include <ostream>
#include <cstdio>
#include <fcntl.h> //renameat
#include <fstream>
#include <sstream>
#include <string>
#include <array>

using namespace Carconfig_base;

void CarConfigFileWriter::writeCarConfig(const std::array<ccValue, cc_no_of_parameters>& allParams) {

    auto ccTempFilePath = carconfig_file_name + "_temp";
    std::ofstream ccTempFile(ccTempFilePath, std::ios_base::out | std::ios_base::trunc);
    if(ccTempFile.fail())
    {
        std::string msg {"CarConfigFileWriter could not open temporary carconfig file: "};
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    for(const auto& value : allParams)
    {
        if (value.status == ccStatus::GOOD)
        {
            ccTempFile << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",0" << std::endl;
        }
        else
        {
            ccTempFile << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",1" << std::endl;
        }
    }
    ccTempFile.close();
    // Swap in the temp file. fd = -1 since they are anyway ignored when file paths are absolute
    renameat(-1, ccTempFilePath.c_str(), -1, carconfig_file_name.c_str());    
}
