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

class MediaClient : public RemoteCtrlClientBase {
  public:
    typedef void (MediaClient::*Func)();

    explicit MediaClient(const std::string& path);
    ~MediaClient();
    void Run();
    void SetMediaPlayback();

  private:
    std::map<std::string, Func> actionMap = {{"Set Media Playback", &MediaClient::SetMediaPlayback}};
    std::vector<std::string> actions;
};

}  // namespace remotectrlsimclient
}  // namespace vcc
