//#define LOG_TAG "hidl-test-server@2.0-service"
#define LOG_TAG "hidl-test-server@1.0-service"

#include "SignalsImpl.h"
#include <utils/Log.h>

namespace vcc
{
namespace vcctest
{
namespace V1_0
{
namespace implementation
{
SignalsImpl::SignalsImpl() : _cb(nullptr) {}
// Methods from ::vcc::vcctest::V1_0::ISignals follow.
Return<uint32_t> SignalsImpl::set(const hidl_string& propValue)
{
  // Call the callback
  if (_cb != nullptr)
  {
    _cb.get()->signalChanged(propValue);
  }
  else
  {
    ALOGE("SignalsImpl::set() callback is null");
  }

  ALOGI("SignalsImpl::set() %s", toString(propValue).c_str());
  return uint32_t{1};
}

Return<void> SignalsImpl::subscribe(const sp<ISignalsCallback>& cb)
{
  _cb = cb;

  return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

ISignals* HIDL_FETCH_ISignals(const char* /* name */) { return new SignalsImpl(); }
}  // namespace implementation
}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc
