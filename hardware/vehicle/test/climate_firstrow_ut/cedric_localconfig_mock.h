/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>

class LocalConfigMock {
  public:
    LocalConfigMock();
    ~LocalConfigMock();

    MOCK_METHOD0(init, bool());
    MOCK_CONST_METHOD1(getIntValue, const int*(const std::string& key));
    MOCK_CONST_METHOD1(getStringValue, const std::string*(const std::string& key));
    MOCK_CONST_METHOD1(getDoubleValue, const double*(const std::string& key));
    MOCK_CONST_METHOD1(getBoolValue, const bool*(const std::string& key));
};
