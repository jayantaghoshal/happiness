#include "signals_server.h"
#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>

#undef LOG_TAG
#define LOG_TAG "DataElementServer"

using namespace ihu::signals::V1_0;

SignalsServer::signal_key SignalsServer::make_key(std::string name, Dir dir){
    return std::make_pair(name, dir);
}

::android::hardware::Return<void> ihu::signals::V1_0::SignalsServer::subscribe(
    const ::android::hardware::hidl_string& signalName,
    Dir dir,
    const ::android::sp<ISignalsChangedCallback>& cb)
{
    ALOGD("SignalsServer::subscribe %s", signalName.c_str());

    const auto key = make_key(signalName, dir);
    auto it = subscriptions.find(key);
    if (it == subscriptions.end())
    {
        auto ins_it = subscriptions.emplace(key, std::vector<::android::sp<ISignalsChangedCallback>>());
        ins_it.first->second.push_back(cb);
    }
    else
    {
        it->second.push_back(cb);
    }

    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> ihu::signals::V1_0::SignalsServer::send(
    const ::android::hardware::hidl_string& signalname,
    Dir dir,
    const ::android::hardware::hidl_string& data)
{
    ALOGD("SignalsServer::send name %s", signalname.c_str());
    ALOGV("SignalsServer::send value %s", data.c_str());

    const auto key = make_key(signalname, dir);
    signalStorage[key] = data;
    auto iter = subscriptions.find(key);
    if (iter != subscriptions.end()) {
        auto& subList = iter->second;
        for (::android::sp<ISignalsChangedCallback>& sub : subList) {
            auto result = sub->signalChanged(signalname, dir, data);
            if (!result.isOk())
            {
                //TODO: Should we remove the subscription?
                ALOGE("Sending signalChanged notification failed, name: %s", signalname.c_str());
            }
        }
    }
    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> ihu::signals::V1_0::SignalsServer::get(
    const ::android::hardware::hidl_string& signalname,
    get_cb _hidl_cb)
{
    ALOGD("SignalsServer::get name:  %s", signalname.c_str());

    Dir dir = Dir::IN; //TODO:Have to update interface to contain DIR
    const auto key = make_key(signalname, dir); 

    auto it = signalStorage.find(key);
    if (it != signalStorage.end())
    {
        const std::string& value = it->second;
        ALOGV("SignalsServer::get value: %s", value.c_str());
        _hidl_cb(value, dir);
    }
    else
    {
        //TODO: Not sure what to do here, if this can even happen..?
        const std::string& value = R"({"timestamp":0, state:0, errorCode: 1 }")";
        ALOGV("SignalsServer::get value not existing, sending dummy: %s", value.c_str());
        _hidl_cb(value, dir);
    }

    return ::android::hardware::Return<void>();
}
