#pragma once

#include <functional>
#include <string>

#include "gen_dataelements.h"
#include "internalsignals.h"

class IDataElementCommBus {
  public:
    static IDataElementCommBus* create();  // the caller of create shall NOT delete the instance when done

    virtual void setNewDataElementHandler(
            std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) = 0;

    virtual void send(const std::string& name, const std::string& payload, autosar::Dir dir) = 0;

    virtual void addName(autosar::Dir dir, const std::string& name) = 0;

    virtual ~IDataElementCommBus() {}
};
