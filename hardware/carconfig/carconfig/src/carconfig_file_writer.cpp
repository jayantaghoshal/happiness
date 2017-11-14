#include "carconfig_file_writer.h"
#include <fcntl.h>  //renameat
#include <sys/stat.h>
#include <array>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "carconfig_base.h"

using namespace Carconfig_base;

void CarConfigFileWriter::writeCarConfig(const std::array<ccValue, cc_no_of_parameters>& allParams) {
    auto ccTempFilePath = carconfig_file_name + "_temp";
    std::ofstream ccTempFile(ccTempFilePath, std::ios_base::out | std::ios_base::trunc);
    if (ccTempFile.fail()) {
        std::string msg{"CarConfigFileWriter could not open temporary carconfig file: "};
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    for (const auto& value : allParams) {
        if (value.status == ccStatus::GOOD) {
            ccTempFile << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",0" << std::endl;
        } else {
            ccTempFile << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",1" << std::endl;
        }
    }
    ccTempFile.close();
    // Swap in the temp file. fd = -1 since they are anyway ignored when file paths are absolute
    renameat(-1, ccTempFilePath.c_str(), -1, carconfig_file_name.c_str());
    // CarConfig is read by other components, set read for group and others
    // SE-Linux will restrict access to the car-config file
    chmod(carconfig_file_name.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}
