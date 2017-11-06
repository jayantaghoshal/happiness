/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <cutils/log.h>
#include "dataelementcommbus.h"

#undef LOG_TAG
#define LOG_TAG "DataElementDBus"

class DataElementCommBus_standalone : public IDataElementCommBus {
  public:
    DataElementCommBus_standalone();

    void setNewDataElementHandler(
            std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) override;
    void addName(autosar::Dir dir, const std::string& name) override;
    void send(const std::string& name, const std::string& payload, autosar::Dir dir) override;
    virtual ~DataElementCommBus_standalone() {}

  private:
    std::function<void(const std::string& name, const std::string& payload)> _callback;
    std::map<std::string, std::string> _datacache;
};

IDataElementCommBus* IDataElementCommBus::create() { return new DataElementCommBus_standalone(); }

DataElementCommBus_standalone::DataElementCommBus_standalone() { ALOGI("Using DataElementCommBus_standalone"); }

void DataElementCommBus_standalone::send(const std::string& name, const std::string& payload, autosar::Dir /*dir*/) {
    // Just call it back directly again
    _datacache[name] = payload;
    if (_callback) {
        _callback(name, payload);
    }
}

void DataElementCommBus_standalone::setNewDataElementHandler(
        std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) {
    _callback = std::move(newDataElementCallback);
}

void DataElementCommBus_standalone::addName(autosar::Dir dir, const std::string& name) {
    (void)dir;
    if (_datacache.find(name) != _datacache.end()) {
        // Yes we have data for this one
        if (_callback) {
            _callback(name, _datacache[name]);
        }
    }
}
