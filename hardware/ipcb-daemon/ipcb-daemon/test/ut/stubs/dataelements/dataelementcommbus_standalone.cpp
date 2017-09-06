/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "dataelementcommbus.h"

#include <QString>
#include <QDebug>

class DataElementCommBus_standalone : public IDataElementCommBus {
public:
    DataElementCommBus_standalone();

    void setNewDataElementHandler(std::function<void(const std::string& name, const QByteArray& payload)>&& newDataElementCallback) override;
    void addName(autosar::Dir dir, const std::string& name) override;
    void send(const std::string& name, const QByteArray& payload, autosar::Dir dir) override;
    virtual ~DataElementCommBus_standalone(){}
    virtual void reset() override {_datacache.clear();} // Only for unittest
    virtual int count() override {return _datacache.size();} // Only for unittest

private:
    std::function<void(const std::string& name, const QByteArray& payload)> _callback;
    std::map<std::string,QByteArray> _datacache;
};

IDataElementCommBus* IDataElementCommBus::create() {
    return new DataElementCommBus_standalone();
}

DataElementCommBus_standalone::DataElementCommBus_standalone() {
    qDebug() << "Using DataElementCommBus_standalone , no mosquitto in other words!!!";
}

void DataElementCommBus_standalone::send(const std::string& name, const QByteArray& payload, autosar::Dir /*dir*/) {
    // Just call it back directly again
    _datacache[name] = payload;
    if (_callback) {
        _callback(name, payload);
    }
}

void DataElementCommBus_standalone::setNewDataElementHandler(std::function<void(const std::string& name, const QByteArray& payload)>&& newDataElementCallback) {
    _callback = std::move(newDataElementCallback);
}

void DataElementCommBus_standalone::addName(autosar::Dir dir, const std::string& name) {
    if (_datacache.find(name)!=_datacache.end()) {
        // Yes we have data for this one
        if (_callback) {
           _callback(name, _datacache[name]);
        }
    }
}
