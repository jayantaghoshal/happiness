#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_

#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

namespace vcc {
/*
 * Describes contract for using LCFG without relying on global state
 */
struct LocalConfigReaderInterface {
    virtual ~LocalConfigReaderInterface() = default;

    // actual backend reaching methods

    /**
     * Get the value of a local config string parameter.
     *
     * Usage:
     *   const std::string my_string = _lcfg->GetString("MyLcfgValue1");
     *
     * If the key is not found a std::runtime_error exception will be thrown.
     *
     * @param key string that identifies a local config value
     * @return String with the requested parameter value.
     */
    virtual std::string GetString(std::initializer_list<std::string> keys_path) const = 0;

    /**
     * Get the value of a local config int parameter.
     *
     * Usage:
     *   const int my_int = _lcfg->GetInt("MyLcfgValue2");
     *
     * If the key is not found a std::runtime_error exception will be thrown.
     *
     * @param key string that identifies a local config value
     * @return int with the requested parameter value.
     */
    virtual int GetInt(std::initializer_list<std::string> keys_path) const = 0;

    /**
     * Get the value of a local config bool parameter.
     *
     * Usage:
     *   const bool my_bool = _lcfg->GetBool("MyLcfgValue3");
     *
     * If the key is not found a std::runtime_error exception will be thrown.
     *
     * @param key string that identifies a local config value
     * @return bool with the requested parameter value.
     */
    virtual bool GetBool(std::initializer_list<std::string> keys_path) const = 0;

    /**
     * Get the value of a local config double parameter.
     *
     * Usage:
     *   const double my_double = _lcfg->GetDouble("MyLcfgValue4");
     *
     * If the key is not found a std::runtime_error exception will be thrown.
     *
     * @param key string that identifies a local config value
     * @return double with the requested parameter value.
     */
    virtual double GetDouble(std::initializer_list<std::string> keys_path) const = 0;

    /**
     * Get the value of a local config array of string parameter.
     *
     * Usage:
     *   const std::vector<string> my_strings = _lcfg->GetStringArray("MyLcfgValue5");
     *
     * If the key is not found a std::runtime_error exception will be thrown.
     *
     * @param key string that identifies a local config value
     * @return vector of strings with requested parameter value
     */
    virtual std::vector<std::string> GetStringArray(std::initializer_list<std::string> keys_path) const = 0;

    // conveniance adapters

    template <class... T>
    std::string GetString(const T &... keys) const {
        return GetString({keys...});
    }

    template <class... T>
    int GetInt(const T &... keys) const {
        return GetInt({keys...});
    }

    template <class... T>
    bool GetBool(const T &... keys) const {
        return GetBool({keys...});
    }

    template <class... T>
    double GetDouble(const T &... keys) const {
        return GetDouble({keys...});
    }

    template <class... T>
    std::vector<std::string> GetStringArray(const T &... keys) const {
        return GetStringArray({keys...});
    }

    template <class R, class... T>
    void GetGenericValue(R *value, const T &... keys) const;

    template <class R, class... T>
    bool TryGetValue(R *value, const T &... keys) const {
        try {
            GetGenericValue(value, keys...);
            return true;
        } catch (const std::runtime_error &) {
            return false;
        }
    }

    template <class R, class... T>
    bool TryGetValueOrDefault(R *value, R default_value, const T &... keys) const {
        try {
            GetGenericValue(value, keys...);
            return true;
        } catch (const std::runtime_error &) {
            *value = default_value;
            return false;
        }
    }
};

namespace detail {
template <class R, class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, R *value, const T &... keys);

template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, std::string *value, const T &... keys) {
    *value = lcfg->GetString(keys...);
}

template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, int32_t *value, const T &... keys) {
    *value = lcfg->GetInt(keys...);
}
template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, uint32_t *value, const T &... keys) {
    *value = lcfg->GetInt(keys...);
}
template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, int16_t *value, const T &... keys) {
    *value = lcfg->GetInt(keys...);
}

template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, uint16_t *value, const T &... keys) {
    *value = lcfg->GetInt(keys...);
}

template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, std::chrono::seconds *value, const T &... keys) {
    *value = std::chrono::seconds(lcfg->GetInt(keys...));
}
template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, std::chrono::milliseconds *value, const T &... keys) {
    *value = std::chrono::milliseconds(lcfg->GetInt(keys...));
}

template <class... T>
void GetValue(const LocalConfigReaderInterface *lcfg, double *value, const T &... keys) {
    *value = lcfg->GetDouble(keys...);
}
}  // namespace detail

template <class R, class... T>
inline void LocalConfigReaderInterface::GetGenericValue(R *value, const T &... keys) const {
    return detail::GetValue(this, value, keys...);
}

}  // namespace vcc

#ifdef GTEST_OS_LINUX_ANDROID
#include <gmock/gmock.h>

namespace vcc {
namespace mocks {
class MockLocalConfigReader : public LocalConfigReaderInterface {
  public:
    MOCK_CONST_METHOD1(GetString, std::string(std::initializer_list<std::string>));
    MOCK_CONST_METHOD1(GetInt, int(std::initializer_list<std::string>));
    MOCK_CONST_METHOD1(GetBool, bool(std::initializer_list<std::string>));
    MOCK_CONST_METHOD1(GetDouble, double(std::initializer_list<std::string>));
    MOCK_CONST_METHOD1(GetStringArray, std::vector<std::string>(std::initializer_list<std::string>));
};

}  // namespace mocks
}  // namespace vcc

#endif

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_ */
