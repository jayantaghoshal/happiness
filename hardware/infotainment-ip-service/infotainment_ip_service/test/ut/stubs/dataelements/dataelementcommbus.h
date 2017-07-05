/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef DATAELEMENTCOMMBUS_H
#define DATAELEMENTCOMMBUS_H

#include <string>
#include <functional>
#include <QByteArray>

#include "gen_dataelements.h"
#include "internalsignals.h"

class IDataElementCommBus {
public:
    static IDataElementCommBus* create(); // the caller of create is responsible to delete when done

    virtual void setNewDataElementHandler(std::function<void(const std::string& name, const QByteArray& payload)>&& newDataElementCallback)=0;

    virtual void send(const std::string& name, const QByteArray& payload, autosar::Dir dir)=0;

    virtual void addName(autosar::Dir dir, const std::string& name)=0;

    virtual ~IDataElementCommBus() {}

    virtual void reset() = 0; // only for UT
    virtual int count() = 0; // only for UT
};

#endif
