/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "temperature_proxy.h"

#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

#include <algorithm>
#include <array>
#include <utility>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

namespace
{
enum class Config
{
    Single,
    Left,
    Right
};

Config carConfigType()
{
    auto hvac = CarConfigParams::CC175_HvacVariantsType::InvalidValue;

    try
    {
        hvac = carconfig::getValue<CarConfigParams::CC175_HvacVariantsType>();
    }
    catch (std::out_of_range& e)
    {
        log_error() << "CC175 not found in carconfig!";
    }

    auto config = Config::Single;

    switch (hvac)
    {
    case CarConfigParams::CC175_HvacVariantsType::HVAC_Small_1_zone:
    case CarConfigParams::CC175_HvacVariantsType::Manual_AC:
        config = Config::Single;
        break;
    case CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_2_zone:
    case CarConfigParams::CC175_HvacVariantsType::Electronic_Climate_Control_4_zone:
    case CarConfigParams::CC175_HvacVariantsType::HVAC_Small_2_zone:
    {
        auto steering = CarConfigParams::CC8_SteeringWheelPositionType::InvalidValue;

        try
        {
            steering = carconfig::getValue<CarConfigParams::CC8_SteeringWheelPositionType>();
        }
        catch (std::out_of_range& e)
        {
            log_error() << "CC8 not found in carconfig!";
        }

        switch (steering)
        {
        case CarConfigParams::CC8_SteeringWheelPositionType::Left_Hand_Drive:
            config = Config::Left;
            break;
        case CarConfigParams::CC8_SteeringWheelPositionType::Right_Hand_Drive:
            config = Config::Right;
            break;
        default:
            log_error() << "Unknown CC8 value: " << static_cast<int>(steering);
            break;
        }
        break;
    }
    default:
        log_error() << "Unknown CC175 value: " << static_cast<int>(hvac);
        break;
    }

    return config;
}

void resetHmiCmptmtTSp(autosar::HmiCmptmtTSp& hmiCmptmtTSp)
{
    auto defaultTemp      = 22.0;
    auto defaultTempHiLoN = autosar::HmiCmptmtTSpSpcl::Norm;

    hmiCmptmtTSp.HmiCmptmtTSpForRowFirstLe     = defaultTemp;
    hmiCmptmtTSp.HmiCmptmtTSpForRowFirstRi     = defaultTemp;
    hmiCmptmtTSp.HmiCmptmtTSpForRowSecLe       = defaultTemp;
    hmiCmptmtTSp.HmiCmptmtTSpForRowSecRi       = defaultTemp;
    hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstLe = defaultTempHiLoN;
    hmiCmptmtTSp.HmiCmptmtTSpSpclForRowFirstRi = defaultTempHiLoN;
    hmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecLe   = defaultTempHiLoN;
    hmiCmptmtTSp.HmiCmptmtTSpSpclForRowSecRi   = defaultTempHiLoN;
}

template <class T>
void unsubscribe(uint16_t id, T sharedObject)
{
    if (id)
    {
        sharedObject.unSubscribe(id);
    }
}
}

TemperatureProxy::TemperatureProxy(NotifiableProperty<double>&                         storedDriverTemp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      storedDriverTempHiLoN,
                                   NotifiableProperty<double>&                         storedPassengerTemp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      storedPassengerTempHiLoN,
                                   NotifiableProperty<double>&                         driverTemp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      driverTempHiLoN,
                                   NotifiableProperty<double>&                         passengerTemp,
                                   NotifiableProperty<autosar::HmiCmptmtTSpSpcl>&      passengerTempHiLoN,
                                   NotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                                   SettingsProxy<double, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>&                              sTemperatureDriverProxy,
                                   SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>&           sTemperatureDriverHiLoNProxy,
                                   SettingsProxy<double, SettingsFramework::UserScope::NOT_USER_RELATED, SettingsFramework::UserScope::NOT_USER_RELATED>&                              sTemperaturePassengerProxy,
                                   SettingsProxy<autosar::HmiCmptmtTSpSpcl, SettingsFramework::UserScope::NOT_USER_RELATED, SettingsFramework::UserScope::NOT_USER_RELATED>&           sTemperaturePassengerHiLoNProxy,
                                   autosar::HmiCmptmtTSp&                              hmiCmptmtTSp)
    : storedDriverTemp_{ storedDriverTemp }
    , storedDriverTempHiLoN_{ storedDriverTempHiLoN }
    , storedPassengerTemp_{ storedPassengerTemp }
    , storedPassengerTempHiLoN_{ storedPassengerTempHiLoN }
    , driverTemp_{ driverTemp }
    , driverTempHiLoN_{ driverTempHiLoN }
    , passengerTemp_{ passengerTemp }
    , passengerTempHiLoN_{ passengerTempHiLoN }
    , maxDefroster_{ maxDefroster }
    , sTemperatureDriverProxy_{ sTemperatureDriverProxy }
    , sTemperatureDriverHiLoNProxy_{ sTemperatureDriverHiLoNProxy }
    , sTemperaturePassengerProxy_{ sTemperaturePassengerProxy }
    , sTemperaturePassengerHiLoNProxy_{ sTemperaturePassengerHiLoNProxy }
    , vehModMngtGlbSafe1_{}
    , hmiCmptmtTSp_{}
    , currentHmiCmptmtTSp_{ hmiCmptmtTSp }
    , subscriptions_{}
    , updateLock_{}
{
    resetHmiCmptmtTSp(currentHmiCmptmtTSp_);

    hmiCmptmtTSp_.send(currentHmiCmptmtTSp_);

    auto config               = carConfigType();
    using UpdateTempFunc      = std::function<void(double)>;
    using UpdateTempHiLoNFunc = std::function<void(autosar::HmiCmptmtTSpSpcl)>;

    UpdateTempFunc      updateDriverTemp{};
    UpdateTempHiLoNFunc updateDriverTempHiLoN{};
    UpdateTempFunc      updatePassengerTemp{};
    UpdateTempHiLoNFunc updatePassengerTempHiLoN{};

    switch (config)
    {
    case Config::Right:
    {
        updateDriverTemp
            = [this](auto temp) { this->updateDriver(temp, currentHmiCmptmtTSp_.HmiCmptmtTSpForRowFirstRi); };
        updateDriverTempHiLoN = [this](auto tempHiLoN) {
            this->updateDriver(tempHiLoN, currentHmiCmptmtTSp_.HmiCmptmtTSpSpclForRowFirstRi);
        };

        updatePassengerTemp
            = [this](auto temp) { this->updatePassenger(temp, currentHmiCmptmtTSp_.HmiCmptmtTSpForRowFirstLe); };
        updatePassengerTempHiLoN = [this](auto tempHiLoN) {
            this->updatePassenger(tempHiLoN, currentHmiCmptmtTSp_.HmiCmptmtTSpSpclForRowFirstLe);
        };
        break;
    }
    case Config::Left:
    {
        updatePassengerTemp
            = [this](auto temp) { this->updatePassenger(temp, currentHmiCmptmtTSp_.HmiCmptmtTSpForRowFirstRi); };
        updatePassengerTempHiLoN = [this](auto tempHiLoN) {
            this->updatePassenger(tempHiLoN, currentHmiCmptmtTSp_.HmiCmptmtTSpSpclForRowFirstRi);
        };

        // Intentional fallthrough
        // Driver position is the same in both Left and Single
    }
    case Config::Single:
    {
        updateDriverTemp
            = [this](auto temp) { this->updateDriver(temp, currentHmiCmptmtTSp_.HmiCmptmtTSpForRowFirstLe); };
        updateDriverTempHiLoN = [this](auto tempHiLoN) {
            this->updateDriver(tempHiLoN, currentHmiCmptmtTSp_.HmiCmptmtTSpSpclForRowFirstLe);
        };

        break;
    }
    default:
        log_error() << "No valid config found!";
        break;
    }

    storedDriverTemp_.set(sTemperatureDriverProxy_.get());
    storedDriverTempHiLoN_.set(sTemperatureDriverHiLoNProxy_.get());

    connect(sTemperatureDriverProxy_, storedDriverTemp_);
    connect(sTemperatureDriverHiLoNProxy_, storedDriverTempHiLoN_);

    updateDriverTemp(storedDriverTemp.get());
    updateDriverTempHiLoN(storedDriverTempHiLoN.get());

    subscriptions_.push_back(driverTemp_.subscribe(std::move(updateDriverTemp)));
    subscriptions_.push_back(driverTempHiLoN_.subscribe(std::move(updateDriverTempHiLoN)));

    auto const passengerPresent = updatePassengerTemp && updatePassengerTempHiLoN;

    if (passengerPresent)
    {

        storedPassengerTemp_.set(sTemperaturePassengerProxy_.get());
        storedPassengerTempHiLoN_.set(sTemperaturePassengerHiLoNProxy_.get());

        connect(sTemperaturePassengerProxy_, storedPassengerTemp_);
        connect(sTemperaturePassengerHiLoNProxy_, storedPassengerTempHiLoN_);

        updatePassengerTemp(storedPassengerTemp.get());
        updatePassengerTempHiLoN(storedPassengerTempHiLoN.get());

        subscriptions_.push_back(passengerTemp_.subscribe(std::move(updatePassengerTemp)));
        subscriptions_.push_back(passengerTempHiLoN_.subscribe(std::move(updatePassengerTempHiLoN)));
    }

    vehModMngtGlbSafe1_.subscribe([this, passengerPresent]() { handleCarMod(passengerPresent); });
}

TemperatureProxy::~TemperatureProxy()
{
}

template <class Setting, class SharedObject>
void TemperatureProxy::connect(Setting& setting, SharedObject& sharedObject)
{
    setting.subscribe([&setting, &sharedObject, this]() { sharedObject.set(setting.get()); });
}

template <typename Value, class Setting>
void TemperatureProxy::update(Value value, Value& signalValue, Setting& setting)
{
    std::lock_guard<std::mutex> lock(updateLock_);

    signalValue = value;

    hmiCmptmtTSp_.send(currentHmiCmptmtTSp_);

    if ((maxDefroster_.get() != FirstRowGen::MaxDefrosterState::ON))
    {
        setting.set(value);
    }
}

void TemperatureProxy::updateDriver(double temp, double& signalTemp)
{
    log_debug() << "Update driver temp: " << driverTemp_.get();
    update(temp, signalTemp, sTemperatureDriverProxy_);
}

void TemperatureProxy::updateDriver(autosar::HmiCmptmtTSpSpcl tempHiLoN, autosar::HmiCmptmtTSpSpcl& signalTempHiLoN)
{
    log_debug() << "Update driver tempHiLoN: " << static_cast<int>(driverTempHiLoN_.get());
    update(tempHiLoN, signalTempHiLoN, sTemperatureDriverHiLoNProxy_);
}

void TemperatureProxy::updatePassenger(double temp, double& signalTemp)
{
    log_debug() << "Update passenger temp: " << passengerTemp_.get();
    update(temp, signalTemp, sTemperaturePassengerProxy_);
}

void TemperatureProxy::updatePassenger(autosar::HmiCmptmtTSpSpcl tempHiLoN, autosar::HmiCmptmtTSpSpcl& signalTempHiLoN)
{
    log_debug() << "Update passenger tempHiLoN: " << static_cast<int>(passengerTempHiLoN_.get());
    update(tempHiLoN, signalTempHiLoN, sTemperaturePassengerHiLoNProxy_);
}

void TemperatureProxy::handleCarMod(bool isPassengerPresent)
{
    if (isPassengerPresent)
    {
        storedPassengerTemp_.set(sTemperaturePassengerProxy_.get());
        storedPassengerTempHiLoN_.set(sTemperaturePassengerHiLoNProxy_.get());
    }

    storedDriverTemp_.set(sTemperatureDriverProxy_.get());
    storedDriverTempHiLoN_.set(sTemperatureDriverHiLoNProxy_.get());
}
