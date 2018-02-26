/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef SETTINGS_PROXY_H
#define SETTINGS_PROXY_H

#include <Application_dataelement.h>
#include <libsettings/setting.h>

template <typename T> 
class SettingsProxyInterface
{
    public:
    virtual ~SettingsProxyInterface() = default;
    virtual bool inCarModeDyno() const;
    virtual T get() const;
    virtual void set(const T& v);
    virtual void subscribe(std::function<void()> callback);
    virtual void unSubscribe();
};

//TODO(climateport): It seems like Setting<T> dynoSetting is not used at all, except for default value, can it be replaced completely with only a default value?

template <typename T, SettingsFramework::UserScope userScope, SettingsFramework::UserScope userScopeDyno>
class SettingsProxy : public SettingsProxyInterface<T>
{
public:
    SettingsProxy(SettingsFramework::Setting<T, userScope> setting, SettingsFramework::Setting<T, userScopeDyno> dynoSetting)
        : setting_(setting)
        , dynoSetting_(dynoSetting)
        , dynoValue_(dynoSetting.defaultValue())
        , callback_(nullptr)
        , isInDyno_(inCarModeDyno())
    {
        vehModMngtGlbSafe1_.subscribe(std::bind(&SettingsProxy::handleSignals, this));
        setting_.setCallback([this](const auto value) {
            normalValue_ = value.value;
            if (!isInDyno_) {
                notify(); 
            }
        });
    }

    virtual ~SettingsProxy() = default;

    bool inCarModeDyno() const
    {
        if( vehModMngtGlbSafe1_.get().isOk() )
        {
            return (vehModMngtGlbSafe1_.get().value().CarModSts1_ == autosar::CarModSts1::CarModDyno);
        }
        return false;
    }

    virtual T get() const
    {
        if (inCarModeDyno())
        {
            return dynoValue_;
        }
        else
        {
            return normalValue_;
            //return setting_.get();        //TODO(climateport): get() function removed
        }
    }

    virtual void set(const T& v)
    {
        if (inCarModeDyno())
        {
            if (v != dynoValue_)
            {
                dynoValue_ = v;
                notify();
            }
        }
        else
        {
            setting_.set(v);
        }
    }
    void subscribe(std::function<void()> callback)
    {
        callback_ = callback;
    }

    void unSubscribe()
    {
        callback_ = nullptr;
    }

private:
    void notify()
    {
        if (callback_)
            callback_();
    }

    void handleSignals()
    {
        auto isDyno = inCarModeDyno();

        if (isDyno != isInDyno_)
        {
            if (isDyno)
            {
                dynoValue_ = dynoSetting_.defaultValue();
            }
            isInDyno_ = isDyno;
            notify();
        }
    }

    SettingsFramework::Setting<T, userScope>        setting_;
    const SettingsFramework::Setting<T, userScopeDyno>  dynoSetting_;
    T                                    normalValue_; 
    T                                    dynoValue_;
    std::function<void()>                callback_;
    bool                                 isInDyno_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
};

#endif // SETTINGS_PROXY_H
