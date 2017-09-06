/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.5.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V1_COM_IHU_DIAGNOSTICS_Diagnostics_PROXY_HPP_
#define V1_COM_IHU_DIAGNOSTICS_Diagnostics_PROXY_HPP_

#include <v1/com/ihu/Diagnostics/DiagnosticsProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif


#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace com {
namespace ihu {
namespace Diagnostics {

template <typename ... _AttributeExtensions>
class DiagnosticsProxy
    : virtual public Diagnostics, 
      virtual public DiagnosticsProxyBase,
      virtual public _AttributeExtensions... {
public:
    DiagnosticsProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~DiagnosticsProxy();

    typedef Diagnostics InterfaceType;



    /**
     * Returns the wrapper class that provides access to the broadcast MessageToApp.
     */
    virtual MessageToAppSelectiveEvent& getMessageToAppSelectiveEvent() {
        static MessageToAppSelectiveEvent event;
        return event;
    }

    /**
     * Calls MessageToDiag with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void MessageToDiag(const uint16_t &_key, const std::vector<uint8_t> &_payload, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls MessageToDiag with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> MessageToDiagAsync(const uint16_t &_key, const std::vector<uint8_t> &_payload, MessageToDiagAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SubscribeOnKeys with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SubscribeOnKeys(const std::vector<uint16_t> &_keys, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SubscribeOnKeys with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SubscribeOnKeysAsync(const std::vector<uint16_t> &_keys, SubscribeOnKeysAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


    /**
     * Returns the CommonAPI address of the remote partner this proxy communicates with.
     */
    virtual const CommonAPI::Address &getAddress() const;

    /**
     * Returns true if the remote partner for this proxy is currently known to be available.
     */
    virtual bool isAvailable() const;

    /**
     * Returns true if the remote partner for this proxy is available.
     */
    virtual bool isAvailableBlocking() const;

    /**
     * Returns the wrapper class that is used to (de-)register for notifications about
     * the availability of the remote partner of this proxy.
     */
    virtual CommonAPI::ProxyStatusEvent& getProxyStatusEvent();

    /**
     * Returns the wrapper class that is used to access version information of the remote
     * partner of this proxy.
     */
    virtual CommonAPI::InterfaceVersionAttribute& getInterfaceVersionAttribute();

 private:
    std::shared_ptr<DiagnosticsProxyBase> delegate_;
};

typedef DiagnosticsProxy<> DiagnosticsProxyDefault;


//
// DiagnosticsProxy Implementation
//
template <typename ... _AttributeExtensions>
DiagnosticsProxy<_AttributeExtensions...>::DiagnosticsProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<DiagnosticsProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<DiagnosticsProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
DiagnosticsProxy<_AttributeExtensions...>::~DiagnosticsProxy() {
}

template <typename ... _AttributeExtensions>
void DiagnosticsProxy<_AttributeExtensions...>::MessageToDiag(const uint16_t &_key, const std::vector<uint8_t> &_payload, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
    delegate_->MessageToDiag(_key, _payload, _internalCallStatus, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> DiagnosticsProxy<_AttributeExtensions...>::MessageToDiagAsync(const uint16_t &_key, const std::vector<uint8_t> &_payload, MessageToDiagAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->MessageToDiagAsync(_key, _payload, _callback, _info);
}
template <typename ... _AttributeExtensions>
void DiagnosticsProxy<_AttributeExtensions...>::SubscribeOnKeys(const std::vector<uint16_t> &_keys, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
    delegate_->SubscribeOnKeys(_keys, _internalCallStatus, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> DiagnosticsProxy<_AttributeExtensions...>::SubscribeOnKeysAsync(const std::vector<uint16_t> &_keys, SubscribeOnKeysAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->SubscribeOnKeysAsync(_keys, _callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &DiagnosticsProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool DiagnosticsProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool DiagnosticsProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& DiagnosticsProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& DiagnosticsProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace Diagnostics
} // namespace ihu
} // namespace com
} // namespace v1



// Compatibility
namespace v1_0 = v1;

#endif // V1_COM_IHU_DIAGNOSTICS_Diagnostics_PROXY_HPP_
