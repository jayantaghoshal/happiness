/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "local_config_file_reader_android_ihu_behavior.h"

#define LOG_TAG "localconfig"
#include <cutils/log.h>

void vcc::LocalConfigFileReaderAndroidIhuBehavior::LoadWithFallback(const std::string& production_path,
                                                                    const std::string& fallback_path,
                                                                    Json::Value* value) {
    try {
        LoadFile(production_path, value);
    } catch (std::exception& ex) {
        ALOGW("Failed to open file %s, for engineering purposes will attempt to fall back to %s",
              production_path.c_str(), fallback_path.c_str());
        LoadFile(fallback_path, value);
    }
}
/*!
 * Instantiates LocalConfigFileReaderAndroidIhuBehavior
 *
 * It mostly creates lambda used for lazy loading local config on first access
 *
 * @param production_path Intended, final product path for localconfig installed from VBF
 * @param fallback_path Engineering, /vendor/ path for default fallback file
 * @param environment_override, name of the environment variable used for overriding the path for test purposes
 */
vcc::LocalConfigFileReaderAndroidIhuBehavior::LocalConfigFileReaderAndroidIhuBehavior(
        const std::string& production_path, const std::string& fallback_path, const char* environment_override)
    : base([production_path, fallback_path, environment_override](Json::Value* value) {

          const char* const environment_vcc_lcfg_path = getenv(environment_override);

          if (environment_vcc_lcfg_path == nullptr) {
              // Default production path
              LoadWithFallback(production_path, fallback_path, value);
          } else {
              ALOGW("Overriding LocalConfig file path with environment variable %s = %s", environment_override,
                    environment_vcc_lcfg_path);
              LoadFile(environment_vcc_lcfg_path, value);
          }
      }) {}
