#pragma once

#include "dataelementcommbus.h"
#include "ihu/signals/1.0/ISignals.h"

///////////////////////////////////////////////////////////////////////////////////////////
/// Hidl implementation of the DataElementCommBus
class DataElementCommBusHIDL : public IDataElementCommBus
{
public:
    void setNewDataElementHandler(
        std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) override;
    void addName(autosar::Dir dir, const std::string& name) override;
    void send(const std::string& name, const std::string& payload, autosar::Dir dir) override;

    static DataElementCommBusHIDL& instance() noexcept;

private:
    DataElementCommBusHIDL();
    virtual ~DataElementCommBusHIDL() = default;

    void VSMStarted();

    //std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessages_;
    ::android::sp<ihu::signals::V1_0::ISignals> proxy_;
};
