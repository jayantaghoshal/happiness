/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus_dbus_receiver.h"
#include <gmock/gmock.h>

// Dont use a google mock here since it seems not be able to methods defined as noexcept
bool DBUS_Rec_Mock_startDBUSEventLoop_Called       = false;
bool DBUS_Rec_Mock_setNewDataElementHandler_Called = false;
bool DBUS_Rec_Mock_subscribeToSignal_Called        = false;
bool DBUS_Rec_Mock_setVSMStartedCB_Called          = false;

DataElementCommBusDBusReceiver& DataElementCommBusDBusReceiver::instance() noexcept
{
    static Pipe                           pipe;
    static DataElementCommBusDBusReceiver dbusReceiver(pipe);
    return dbusReceiver;
}

DataElementCommBusDBusReceiver::DataElementCommBusDBusReceiver(Pipe& pipe) noexcept : _wakeUpPipe(pipe)
{
    DBUS_Rec_Mock_startDBUSEventLoop_Called       = false;
    DBUS_Rec_Mock_setNewDataElementHandler_Called = false;
    DBUS_Rec_Mock_subscribeToSignal_Called        = false;
    DBUS_Rec_Mock_setVSMStartedCB_Called          = false;
}

DataElementCommBusDBusReceiver::~DataElementCommBusDBusReceiver() noexcept
{
}

bool DataElementCommBusDBusReceiver::startDBUSEventLoop() noexcept
{
    DBUS_Rec_Mock_startDBUSEventLoop_Called = true;
    return true;
}

void DataElementCommBusDBusReceiver::subscribeToSignal(autosar::Dir dir, const std::string& name) noexcept
{
    DBUS_Rec_Mock_subscribeToSignal_Called = true;
}

void DataElementCommBusDBusReceiver::setNewDataElementHandler(
    std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) noexcept
{
    DBUS_Rec_Mock_setNewDataElementHandler_Called = true;
}

void DataElementCommBusDBusReceiver::setVSMStartedCB(std::function<void(void)>&& vsmStartedCallback) noexcept
{
    DBUS_Rec_Mock_setVSMStartedCB_Called = true;
}

void DataElementCommBusDBusReceiver::exit()
{
}

