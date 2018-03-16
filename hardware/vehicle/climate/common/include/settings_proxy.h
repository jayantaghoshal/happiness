/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include "logging_context.h"

template <typename T>
class SettingsProxyInterface {
  public:
    virtual ~SettingsProxyInterface() = default;
    virtual bool inCarModeDyno() const;
    virtual T defaultValuePORTHELPER() const;
    virtual void set(const T& v);
    virtual void subscribe(std::function<void(T)> callback);
    virtual void unSubscribe();
};

// TODO(climateport): It seems like Setting<T> dynoSetting is not used at all, except for default value, can it be
// replaced completely with only a default value?

template <typename T, SettingsFramework::UserScope userScope, SettingsFramework::UserScope userScopeDyno>
class SettingsProxy : public SettingsProxyInterface<T> {
  public:
    SettingsProxy(SettingsFramework::Setting<T, userScope>& setting,
                  SettingsFramework::Setting<T, userScopeDyno>& dynoSetting)
        : setting_(setting),
          dynoSetting_(dynoSetting),
          dynoValue_(dynoSetting.defaultValue()),
          callback_(nullptr),
          isInDyno_(inCarModeDyno()) {
        vehModMngtGlbSafe1_.subscribe(std::bind(&SettingsProxy::handleSignals, this));
        log_info() << "Settingsproxy " << static_cast<int>(setting_.name_) << " ctor isdyno " << isInDyno_;
        setting_.setCallback([this](const auto value) {
            log_info() << "Settingsproxy " << static_cast<int>(setting_.name_) << " callback";
            normalValue_ = value.value;
            if (!isInDyno_) {
                notify();
            }
        });

        // TODO(climateport): Subscribing to dyno setting makes no sense but some old unit tests depend on it
        dynoSetting_.setCallback([this](const auto value) {
            log_info() << "Settingsproxy " << static_cast<int>(setting_.name_) << " dynocallback";
            dynoValue_ = value.value;
            if (isInDyno_) {
                notify();
            }
        });
    }

    virtual ~SettingsProxy() = default;

    bool inCarModeDyno() const {
        if (vehModMngtGlbSafe1_.get().isOk()) {
            return (vehModMngtGlbSafe1_.get().value().CarModSts1_ == autosar::CarModSts1::CarModDyno);
        }
        return false;
    }

    virtual T defaultValuePORTHELPER() const { return setting_.defaultValue(); };

    virtual void set(const T& v) {
        if (inCarModeDyno()) {
            log_info() << "Settingsproxy" << static_cast<int>(setting_.name_) << " set in dyno";
            if (v != dynoValue_) {
                dynoValue_ = v;
                notify();
            }
        } else {
            log_info() << "Settingsproxy" << static_cast<int>(setting_.name_) << " set outside dyno";
            setting_.set(v);
        }
    }
    void subscribe(std::function<void(T)> callback) {
        log_info() << "Settingsproxy" << static_cast<int>(setting_.name_) << " subscribe";
        callback_ = callback;
        callback_(get());
    }

    void unSubscribe() { callback_ = nullptr; }

  private:
    T get() const {
        if (inCarModeDyno()) {
            return dynoValue_;
        } else {
            return normalValue_;
            // return setting_.get();        //TODO(climateport): get() function removed
        }
    }

    void notify() {
        if (callback_) {
            log_info() << "Settingsproxy" << static_cast<int>(setting_.name_) << " notify";
            callback_(get());
        }
    }

    void handleSignals() {
        auto isDyno = inCarModeDyno();

        if (isDyno != isInDyno_) {
            log_info() << "Settingsproxy" << static_cast<int>(setting_.name_) << " change isDyno= " << isDyno;

            if (isDyno) {
                dynoValue_ = dynoSetting_.defaultValue();
            }
            isInDyno_ = isDyno;
            notify();
        }
    }

    SettingsFramework::Setting<T, userScope>& setting_;
    SettingsFramework::Setting<T, userScopeDyno>& dynoSetting_;
    T normalValue_;
    T dynoValue_;
    std::function<void(T)> callback_;
    bool isInDyno_;

    // Flexray signals
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
};
