/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.5.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_ORG_VOLVOCARS_VOLVOONCALL_VOLVO_ON_CALL_BACKEND_HPP_
#define V0_ORG_VOLVOCARS_VOLVOONCALL_VOLVO_ON_CALL_BACKEND_HPP_




#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Types.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace volvocars {
namespace VolvoOnCall {

class VolvoOnCallBackend {
public:
    virtual ~VolvoOnCallBackend() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
};

const char* VolvoOnCallBackend::getInterface() {
    return ("org.volvocars.VolvoOnCall.VolvoOnCallBackend");
}

CommonAPI::Version VolvoOnCallBackend::getInterfaceVersion() {
    return CommonAPI::Version(0, 1);
}


} // namespace VolvoOnCall
} // namespace volvocars
} // namespace org
} // namespace v0

namespace CommonAPI {
}


// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_VOLVOCARS_VOLVOONCALL_VOLVO_ON_CALL_BACKEND_HPP_
