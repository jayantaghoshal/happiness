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

class AudioClient : public RemoteCtrlClientBase {
  public:
    typedef void (AudioClient::*Func)();
    AudioClient(std::string path);
    ~AudioClient();
    void Run();
    void GetAudioVolume();
    void SetAudioVolume();

  private:
    std::map<std::string, Func> actionMap = {{"Get Audio Volume", &AudioClient::GetAudioVolume},
                                             {"Set Audio Volume", &AudioClient::SetAudioVolume}};

    std::vector<std::string> actions;
};

}  // namespace remotectrlsimclient
}  // namespace vcc
