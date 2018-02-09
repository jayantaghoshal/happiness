/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "local_config_reader.h"

#include <fstream>

#define LOG_TAG "localconfig"
#include <cutils/log.h>

using vcc::LocalConfigReader;

namespace {
std::string GetKeyString(std::initializer_list<std::string> keys) {
    std::stringstream key_string;
    for (auto& k : keys) {
        key_string << "[\"" << k << "\"]";
    }
    return key_string.str();
}
}

void vcc::LocalConfigReader::Preload() {
    std::unique_lock<std::mutex> lock(mutex_);
    loader_(&root_);
}

const Json::Value& LocalConfigReader::GetJsonValue(std::initializer_list<std::string> keys) const {
    if (keys.size() == 0) throw std::runtime_error("At least one key must be requested from LocalConfig");

    if (root_.isNull()) loader_(&root_);

    const Json::Value* current_node = &root_;
    const Json::Value* node_value = nullptr;
    for (auto& key : keys) {
        node_value = &(*current_node)[key];
        if (node_value->isNull()) {
            auto message = "Parameter " + GetKeyString(keys) + " not found in localconfig.";
            ALOGE("%s", message.c_str());
            throw std::runtime_error(message);
        }
        current_node = node_value;
    }

    return *node_value;  // NOLINT (loop is runs at least once due to arguments validation, it is not null then)
}

#define THROW_TYPE_MISMATCH(expected_typename)                                                 \
    do {                                                                                       \
        auto message = "Parameter " + GetKeyString(keys) + " is not a " expected_typename "."; \
        ALOGE("%s", message.c_str());                                                          \
        throw std::runtime_error(message);                                                     \
    } while (false)

std::string LocalConfigReader::GetString(std::initializer_list<std::string> keys) const {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value& node_value = GetJsonValue(keys);
    if (!node_value.isString()) THROW_TYPE_MISMATCH("std::string");

    return node_value.asString();
}

int LocalConfigReader::GetInt(std::initializer_list<std::string> keys) const {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value& node_value = GetJsonValue(keys);

    if (!node_value.isInt()) THROW_TYPE_MISMATCH("int");

    return node_value.asInt();
}

bool LocalConfigReader::GetBool(std::initializer_list<std::string> keys) const {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value& node_value = GetJsonValue(keys);

    if (!node_value.isBool()) THROW_TYPE_MISMATCH("bool");

    return node_value.asBool();
}

double LocalConfigReader::GetDouble(std::initializer_list<std::string> keys) const {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value& node_value = GetJsonValue(keys);

    if (!node_value.isDouble()) THROW_TYPE_MISMATCH("double");

    return node_value.asDouble();
}

std::vector<std::string> LocalConfigReader::GetStringArray(std::initializer_list<std::string> keys) const {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value& array = GetJsonValue(keys);

    if (!array.isArray()) THROW_TYPE_MISMATCH("array");

    std::vector<std::string> values;
    for (const Json::Value& val : array) {
        if (!val.isString()) {
            auto message = "List node_value for parameter " + GetKeyString(keys) + " is not a string.";
            ALOGE("%s", message.c_str());
            throw std::runtime_error(message);
        }
        values.push_back(val.asString());
    }

    return values;
}

void vcc::LocalConfigReader::LoadFile(const std::string& file_path, Json::Value* value) {
    std::ifstream ifs(file_path);
    if (!ifs) {
        throw std::runtime_error("JSON file " + file_path + " could not be opened.");
    }

    Json::Reader reader;
    if (!reader.parse(ifs, *value)) {
        throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
    }
}

vcc::LocalConfigFileReader::LocalConfigFileReader(const std::string& file_path)
    : base([file_path](Json::Value* value) { LoadFile(file_path, value); }) {}

vcc::LocalConfigStaticContentReader::LocalConfigStaticContentReader(const std::string& json)
    : base([json](Json::Value* value) {
          Json::Reader reader;
          if (!reader.parse(json, *value)) {
              throw std::runtime_error("JSON text could not be parsed, please check content.");
          }
      }) {}
