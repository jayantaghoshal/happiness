/*===========================================================================*\
 * Copyright 2017 Volvo Car Corporation, All Rights Reserved.
 \*==========================================================================*/

#ifndef VCC_LOCALCONFIG_TEST_H_
#define VCC_LOCALCONFIG_TEST_H_

#include <string>

namespace vcc {
namespace localconfig {

/**
   Initialize the use of localconfig in a test scenario

   @note This function should only be used in test scenarios.

   @note If the file can't be loaded a std::runtime_error exception will be thrown.

   @param filepath Absolute path to localconfig json file.
*/
void TestInit(const std::string &file_path);

}  // namespace localconfig
}  // namespace vcc

#endif  // VCC_LOCALCONFIG_TEST_H_
