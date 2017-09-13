#include "signals_server.h"
#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include <cctype>

#undef LOG_TAG
#define LOG_TAG "DataElementServer"

using namespace vendor::volvocars::hardware::signals::V1_0;
using namespace vendor::volvocars::hardware::signals::V1_0::implementation;

std::regex buildRegexFromFilter(const std::string& filter) {    
    std::string regexStr = std::regex_replace(filter, std::regex("\\*"), ".*");
    regexStr = "^" + regexStr + "$";
    return std::regex(regexStr);
}

WildCardSubscription::WildCardSubscription(const std::string filter, const Dir dir, ::android::sp<ISignalsChangedCallback> callback): 
    filter{filter}, 
    regexFilter{buildRegexFromFilter(filter)}, 
    dir{dir},
    callback{callback} {
}

SignalsServer::signal_key SignalsServer::make_key(std::string name, Dir dir){
    return std::make_pair(name, dir);
}

bool isValidName(const std::string& name) {
    if (name.size() == 0)
    {
        ALOGE("Invalid name, can not be empty string");
        return false;
    }

    for (const auto& c : name) {        
        if (!isalnum(c) && c != '_')
        {
            ALOGE("Invalid name: %s", name.c_str());
            return false;
        }
    }
    return true;
}
bool isValidFilter(const std::string signalName, bool& contains_wildcard_out) {
    contains_wildcard_out = false;

    if (signalName.size() == 0)
    {
        ALOGE("Invalid filter, can not be empty string");
        false;
    }

    for (const auto& c : signalName) {        
        if (!isalnum(c) && c != '*' && c != '_')
        {
            ALOGE("Invalid filter: %s", signalName.c_str());
            return false;
        }
        if (c == '*') {
            contains_wildcard_out = true;
        }        
    }
    return true;
}

::android::hardware::Return<void> SignalsServer::subscribe(
    const ::android::hardware::hidl_string& signalName,
    Dir dir,
    const ::android::sp<ISignalsChangedCallback>& cb)
{    
    ALOGD("SignalsServer::subscribe %s", signalName.c_str());
    

    bool containsWildcard = false;
    const bool validFilter = isValidFilter(signalName, containsWildcard);
    if (!validFilter)
    {
        return ::android::hardware::Return<void>();
    }

    if (containsWildcard)
    {
        wildcard_subscriptions.emplace_back(signalName, dir, cb);
        ALOGD("Number of wildcard subscribers: %lu", wildcard_subscriptions.size());
    }
    else 
    {
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
    }
    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> SignalsServer::send(
    const ::android::hardware::hidl_string& signalname,
    Dir dir,
    const ::android::hardware::hidl_string& data)
{
    ALOGD("SignalsServer::send name %s", signalname.c_str());
    if (!isValidName(signalname)) {        
        return ::android::hardware::Return<void>();
    }
    ALOGV("SignalsServer::send value %s", data.c_str());

    {
        //Exact match subscriptions

        const auto key = make_key(signalname, dir);
        signalStorage[key] = data;
        auto map_iter = subscriptions.find(key);
        if (map_iter != subscriptions.end()) {
            auto& subList = map_iter->second;
            
            for (auto subList_it = subList.begin(); subList_it != subList.end(); ) {        
                auto sub = *subList_it;
                auto result = sub->signalChanged(signalname, dir, data);
                
                if (result.isDeadObject()) {
                    subList_it = subList.erase(subList_it);
                    // isOk will be = false here but we anyway need to check isOk to avoid killing this process
                    // https://source.android.com/devices/architecture/hidl-cpp/functions
                    result.isOk();
                    continue;
                }
                else 
                {
                    ++subList_it;
                }

                if (!result.isOk())
                {
                    ALOGE("Send signalChanged notification failed, name: %s, description: %s", signalname.c_str(), result.description().c_str());
                }
            }
        }
    }
    {
        //Wildcard subscriptions

        for (auto it = wildcard_subscriptions.begin(); it != wildcard_subscriptions.end(); ) {        
            std::cmatch match;
            if (dir != it->dir || !std::regex_search(signalname.c_str(), match, it->regexFilter))
            {
                ++it;
                continue;
            }
            auto result = it->callback->signalChanged(signalname, dir, data);            
            if (result.isDeadObject()) {
                it = wildcard_subscriptions.erase(it);
                // isOk will be = false here but we anyway need to check isOk to avoid killing this process
                // https://source.android.com/devices/architecture/hidl-cpp/functions
                result.isOk();
                continue;
            }
            else
            {
                ++it;
            }

            if (!result.isOk())
            {
                ALOGE("Send signalChanged notification failed, name: %s, description: %s", signalname.c_str(), result.description().c_str());
            }

        }
    }
    return ::android::hardware::Return<void>();
}


::android::hardware::Return<void> SignalsServer::get(
    const ::android::hardware::hidl_string& signalname, 
    Dir dir, 
    get_cb _hidl_cb) 
{
    ALOGD("SignalsServer::get name: %s", signalname.c_str());

    const auto key = make_key(signalname, dir); 

    auto it = signalStorage.find(key);
    if (it != signalStorage.end())
    {
        const std::string& value = it->second;
        ALOGV("SignalsServer::get value: %s", value.c_str());
        _hidl_cb(value);
    }
    else
    {
        ALOGD("SignalsServer::get name not existing: %s", signalname.c_str());
        _hidl_cb("");
    }

    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> SignalsServer::get_all(
    const ::android::hardware::hidl_string& filter, 
    const Dir dir, 
    get_all_cb _hidl_cb) 
{
    ALOGD("SignalsServer::get_all: %s", filter.c_str());
    bool containsWildcard_ignore;
    if (!isValidFilter(filter, containsWildcard_ignore)) {
        return ::android::hardware::Return<void>();
    }

    std::regex r = buildRegexFromFilter(filter);

    std::vector<Result> result;    
    for (const auto& s : signalStorage) {
        std::cmatch match;
        if (dir != s.first.second || !std::regex_search(s.first.first.c_str(), match, r)) {
            continue;
        }
        Result r;
        r.name = s.first.first;
        r.dir = s.first.second;
        r.value = s.second;
        result.push_back(r);        
    }    
    _hidl_cb(result);
    return ::android::hardware::Return<void>();
}
