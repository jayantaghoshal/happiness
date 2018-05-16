/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <stdexcept>
#include <string>

namespace vcc {
namespace remotectrl {

class RemoteCtrlError : public std::exception {
  public:
    explicit RemoteCtrlError(const std::string& what) : error_(what) {}

    const char* what() const noexcept override { return error_.what(); }

  private:
    std::runtime_error error_;
};

class RemoteCtrlMsgLengthError : public RemoteCtrlError {
  public:
    RemoteCtrlMsgLengthError(const std::string& method_name, const uint32_t& expected_len, const uint32_t& actual_len)
        : RemoteCtrlError("RemoteCtrlMsgLengthError: <" + method_name + "> expected: " + std::to_string(expected_len) +
                          " actual: " +
                          std::to_string(actual_len)) {}
};

class RemoteCtrlParamRangeError : public RemoteCtrlError {
  public:
    RemoteCtrlParamRangeError(const std::string& method_name,
                              const std::string& param,
                              const uint8_t& max_allowed,
                              const uint8_t& actual_requested)
        : RemoteCtrlError("RemoteCtrlParamRangeError: <" + method_name + "> for '" + param + "' allowed range is [0," +
                          std::to_string(max_allowed) +
                          "], actual requested " +
                          std::to_string(actual_requested)) {}
};

class RemoteCtrlConversionError : public RemoteCtrlError {
  public:
    RemoteCtrlConversionError(const std::string& what) : RemoteCtrlError("RemoteCtrlConversionError: " + what) {}
};

class RemoteCtrlInvalidArgument : public RemoteCtrlError {
  public:
    RemoteCtrlInvalidArgument(const std::string& what) : RemoteCtrlError("RemoteCtrlInvalidArgument: " + what) {}
};

}  // namespace remotectrl
}  // namespace vcc
