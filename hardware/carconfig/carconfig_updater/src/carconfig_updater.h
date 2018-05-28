/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "HisipClient.h"
#include "carconfig_base.h"
#include "diagnostics_reporter.h"
#include "vipcom_client.h"

class CarConfigUpdater {
  public:
    static int32_t runUpdater();

  private:
    static void frameReceiver(Carconfig_base::CarConfigList& buff, uint32_t timeout);

    static void writeEmptyFile(std::string filePath);

    static void checkReceivedValues(const std::map<int, std::vector<int>> ccParamList,
                                    Carconfig_base::CarConfigList& buff,
                                    bool& allParamsReceived,
                                    bool& allParamsOK,
                                    std::map<uint32_t, uint8_t>& errorList);

    static void checkExistingParams(const std::map<int, std::vector<int>> ccParamList,
                                    bool& allParamsOK,
                                    std::map<uint32_t, uint8_t>& errorList);

    static bool storeReceivedParameter(Carconfig_base::CarConfigList& buffer);

    static bool setStateAndSendDiagnostics(
            bool stateConfigured,
            bool allParamsReceived,
            bool allParamsOk,
            bool paramsChanged,
            bool allStoredParamsOk,
            std::map<uint32_t, uint8_t> receivedBadParams,
            std::map<uint32_t, uint8_t> storedBadParams,  // diagnosticsClient &diagClient, //TODO add diagnostics
            vcc::carconfig::DiagnosticsReporter* diag_rep,
            bool& rebootNeeded);

    static std::vector<uint8_t> carconfigParamFaultsPack(std::map<uint32_t, uint8_t> params);

    static void updateVipParameters(CarConfigVipCom& vipcomClient);
};
