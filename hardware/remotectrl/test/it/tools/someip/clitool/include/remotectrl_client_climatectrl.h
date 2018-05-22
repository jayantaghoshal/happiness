/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "remotectrl_client_base.h"

namespace vcc {
namespace remotectrlsimclient {

using namespace vcc::remotectrlsim;

class ClimateClient : public RemoteCtrlClientBase {
  public:
    typedef void (ClimateClient::*Func)();

    ClimateClient(std::string path);

    ~ClimateClient();

    void Run();

    void GetFanLevel();

    void SetFanLevel();

    void GetMaxDefrost();

    void SetMaxDefrost();

    void GetAcState();

    void SetAcState();

    void GetTemperature();

    void SetTemperature();

  private:
    std::map<std::string, Func> actionMap = {{"Get Fan Level", &ClimateClient::GetFanLevel},
                                             {"Set Fan Level", &ClimateClient::SetFanLevel},
                                             {"Get Max Defrost", &ClimateClient::GetMaxDefrost},
                                             {"Set Max Defrost", &ClimateClient::SetMaxDefrost},
                                             {"Get Ac State", &ClimateClient::GetAcState},
                                             {"Set Ac State", &ClimateClient::SetAcState},
                                             {"Get Temperature", &ClimateClient::GetTemperature},
                                             {"Set Temperature", &ClimateClient::SetTemperature}};

    std::vector<std::string> actions;
};

}  // namespace remotectrlsimclient
}  //