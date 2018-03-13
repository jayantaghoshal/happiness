/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "carconfig_file_writer.h"
#include "carconfig_reader.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Error, number of input argument must be 2\n");
        return 1;
    }
    int ccParam = std::stoi(argv[1]);
    int ccVal = std::stoi(argv[2]);

    CarConfigReader ccReader;
    std::array<Carconfig_base::ccValue, Carconfig_base::cc_no_of_parameters> ccParameters;
    Carconfig_base::ccValue value;

    for (int i = 1; i <= Carconfig_base::cc_no_of_parameters; i++) {
        value.raw = ccReader.getRawValue(i);
        value.subs = ccReader.getValue(i);
        value.status = ccReader.getStatus(i);
        ccParameters[i - 1] = value;
    }

    value.raw = ccVal;
    value.subs = ccVal;
    value.status = Carconfig_base::ccStatus::GOOD;
    ccParameters[ccParam - 1] = value;
    CarConfigFileWriter::writeCarConfig(ccParameters);
    return 0;
}
