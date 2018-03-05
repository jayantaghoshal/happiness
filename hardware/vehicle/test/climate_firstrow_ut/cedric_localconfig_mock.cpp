/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <assert.h>
#include <cedric_localconfig_mock.h>
#include <vcc/localconfig.h>

static LocalConfigMock* localConfigMock;

LocalConfigMock::LocalConfigMock() {
    assert(localConfigMock == nullptr);
    localConfigMock = this;
}

LocalConfigMock::~LocalConfigMock() { localConfigMock = nullptr; }

namespace vcc {
namespace localconfig {
bool init(void) {
    assert(localConfigMock != nullptr);
    return localConfigMock->init();
}

template <>
int GetInt(const std::string& key) {
    auto* p = localConfigMock->getIntValue(key);
    assert(p != nullptr);
    return *p;
}

template <>
std::string GetString(const std::string& key) {
    auto* p = localConfigMock->getStringValue(key);
    assert(p != nullptr);
    return *p;
}

template <>
double GetDouble(const std::string& key) {
    auto* p = localConfigMock->getDoubleValue(key);
    assert(p != nullptr);
    return *p;
}

template <>
bool GetBool(const std::string& key) {
    auto* p = localConfigMock->getBoolValue(key);
    assert(p != nullptr);
    return *p;
}
}  // namespace localconfig
}  // namespace vcc
