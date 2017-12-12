/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "carconfig_file_reader.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <istream>
#include <sstream>
#include <stdexcept>
#include "carconfig_base.h"

using namespace Carconfig_base;

CarConfigFileReader::CarConfigFileReader() {
    filePos_ = 0;
    ccFile_ = std::unique_ptr<std::fstream>(new std::fstream);
    fileBuf_ = std::unique_ptr<std::stringstream>(new std::stringstream);
}

void CarConfigFileReader::open(const std::string filePath) {
    filePath_ = filePath;
    ccFile_->open(filePath_, std::ios::in);
    if (ccFile_->fail()) {
        throw std::runtime_error("CarConfigFileReader could not open carconfig file: " + filePath + " errno: " +
                                 strerror(errno));
    }
    *fileBuf_ << ccFile_->rdbuf();
}

ccValue CarConfigFileReader::getValue(int id) {
    std::string line;
    ccValue value;

    if ((id < 1) || (id > cc_no_of_parameters)) {
        throw std::runtime_error("carconfig parameter " + std::to_string(id) + " out of range");
    }
    if (id < filePos_) {
        fileBuf_->seekg(0);
        filePos_ = 0;
    }
    if (id > filePos_ + 1) {
        while (std::getline(*fileBuf_, line)) {
            filePos_++;
            if (id == filePos_ + 1) {
                break;
            }
        }
    }
    std::string cell;
    std::getline(*fileBuf_, line);

    std::stringstream lineStream(line);
    filePos_++;

    std::getline(lineStream, cell, ',');
    if (!validateValue(std::stoi(cell), value.raw)) {
        throw std::runtime_error("Out of range 'Raw' value CC" + std::to_string(id));
    }

    std::getline(lineStream, cell, ',');
    if (!validateValue(std::stoi(cell), value.subs)) {
        throw std::runtime_error("Out of range 'Substitute' value CC" + std::to_string(id));
    }

    std::getline(lineStream, cell, ',');
    if (!validateStatus(std::stoi(cell), value.status)) {
        throw std::runtime_error("Out of range 'Status' value CC" + std::to_string(id));
    }

    return value;
}

bool CarConfigFileReader::validateValue(int unvalidatedValue, uint8_t &validatedValue) {
    if ((unvalidatedValue >= 0) && (unvalidatedValue <= 255)) {
        validatedValue = static_cast<uint8_t>(unvalidatedValue);
        return true;
    } else {
        return false;
    }
}

bool CarConfigFileReader::validateStatus(int unvalidatedStatus, ccStatus &validatedStatus) {
    if (unvalidatedStatus == 0) {
        validatedStatus = ccStatus::GOOD;
        return true;
    } else if (unvalidatedStatus == 1) {
        validatedStatus = ccStatus::INVALID;
        return true;
    }

    validatedStatus = ccStatus::INVALID;
    return false;
}
