/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.5.v201512091508.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
/**
 * description: This is the FB OTA interface on IP Command Bus
 */
#ifndef V0_ORG_VOLVO_CONNECTIVITY_OTA_PROXY_HPP_
#define V0_ORG_VOLVO_CONNECTIVITY_OTA_PROXY_HPP_

#include <v0/org/volvo/connectivity/OTAProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif


#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace volvo {
namespace connectivity {

template <typename ... _AttributeExtensions>
class OTAProxy
    : virtual public OTA, 
      virtual public OTAProxyBase,
      virtual public _AttributeExtensions... {
public:
    OTAProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~OTAProxy();

    typedef OTA InterfaceType;



    /**
     * Returns the wrapper class that provides access to the broadcast DownloadStatusNotification.
     */
    virtual DownloadStatusNotificationEvent& getDownloadStatusNotificationEvent() {
        return delegate_->getDownloadStatusNotificationEvent();
    }

    /**
     * description: DownloadStatus (0x0B01)
                          This signal is sent by the VCM
     *   when status of current
                          download or installation is
     *   changed. It shall be used to
                          populate the OTA HMI.
     * Calls DownloadStatusRequest with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void DownloadStatusRequest(CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, ::v0::org::volvo::connectivity::OTATypes::DownloadStatus &_downloadStatus, std::vector<::v0::org::volvo::connectivity::OTATypes::SoftwareProduct> &_downloadList, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls DownloadStatusRequest with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> DownloadStatusRequestAsync(DownloadStatusRequestAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: DownloadSoftware (0x0B02)
                          This operation is used when IHU
     *   HMI requests VCM to start
                          downloading software.
     * Calls DownloadSoftwareSetRequest with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void DownloadSoftwareSetRequest(CommonAPI::CallStatus &_internalCallStatus, std::vector<::v0::org::volvo::connectivity::OTATypes::UUID> &_downloadList, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls DownloadSoftwareSetRequest with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> DownloadSoftwareSetRequestAsync(DownloadSoftwareSetRequestAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: DeletedSoftware (0x0B03)
                          This operation is used when IHU
     *   requests VCM to inform
                          VCC back end systems that
     *   software has been deleted.
     * Calls DeleteStatusSetRequest with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void DeleteStatusSetRequest(CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, std::vector<::v0::org::volvo::connectivity::OTATypes::PartIdentifier> &_deletedSoftware, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls DeleteStatusSetRequest with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> DeleteStatusSetRequestAsync(DeleteStatusSetRequestAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: CancelDownload (0x0B05)
             This operation is used when IHU HMI requests
     *   VCM to cancel a currently ongoing download of software.
     * Calls CancelDownloadSetRequest with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void CancelDownloadSetRequest(const ::v0::org::volvo::connectivity::OTATypes::UUID &_software, CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, ::v0::org::volvo::connectivity::OTATypes::GenericOkStatus &_status, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls CancelDownloadSetRequest with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> CancelDownloadSetRequestAsync(const ::v0::org::volvo::connectivity::OTATypes::UUID &_software, CancelDownloadSetRequestAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


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
    std::shared_ptr<OTAProxyBase> delegate_;
};

typedef OTAProxy<> OTAProxyDefault;


//
// OTAProxy Implementation
//
template <typename ... _AttributeExtensions>
OTAProxy<_AttributeExtensions...>::OTAProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<OTAProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<OTAProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
OTAProxy<_AttributeExtensions...>::~OTAProxy() {
}

/**
 * description: DownloadStatus (0x0B01)
                      This signal is sent by the VCM
 *   when status of current
                      download or installation is
 *   changed. It shall be used to
                      populate the OTA HMI.
 */
template <typename ... _AttributeExtensions>
void OTAProxy<_AttributeExtensions...>::DownloadStatusRequest(CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, ::v0::org::volvo::connectivity::OTATypes::DownloadStatus &_downloadStatus, std::vector<::v0::org::volvo::connectivity::OTATypes::SoftwareProduct> &_downloadList, const CommonAPI::CallInfo *_info) {
    delegate_->DownloadStatusRequest(_internalCallStatus, _error, _downloadStatus, _downloadList, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> OTAProxy<_AttributeExtensions...>::DownloadStatusRequestAsync(DownloadStatusRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->DownloadStatusRequestAsync(_callback, _info);
}
/**
 * description: DownloadSoftware (0x0B02)
                      This operation is used when IHU
 *   HMI requests VCM to start
                      downloading software.
 */
template <typename ... _AttributeExtensions>
void OTAProxy<_AttributeExtensions...>::DownloadSoftwareSetRequest(CommonAPI::CallStatus &_internalCallStatus, std::vector<::v0::org::volvo::connectivity::OTATypes::UUID> &_downloadList, const CommonAPI::CallInfo *_info) {
    delegate_->DownloadSoftwareSetRequest(_internalCallStatus, _downloadList, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> OTAProxy<_AttributeExtensions...>::DownloadSoftwareSetRequestAsync(DownloadSoftwareSetRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->DownloadSoftwareSetRequestAsync(_callback, _info);
}
/**
 * description: DeletedSoftware (0x0B03)
                      This operation is used when IHU
 *   requests VCM to inform
                      VCC back end systems that
 *   software has been deleted.
 */
template <typename ... _AttributeExtensions>
void OTAProxy<_AttributeExtensions...>::DeleteStatusSetRequest(CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, std::vector<::v0::org::volvo::connectivity::OTATypes::PartIdentifier> &_deletedSoftware, const CommonAPI::CallInfo *_info) {
    delegate_->DeleteStatusSetRequest(_internalCallStatus, _error, _deletedSoftware, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> OTAProxy<_AttributeExtensions...>::DeleteStatusSetRequestAsync(DeleteStatusSetRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->DeleteStatusSetRequestAsync(_callback, _info);
}
/**
 * description: CancelDownload (0x0B05)
         This operation is used when IHU HMI requests
 *   VCM to cancel a currently ongoing download of software.
 */
template <typename ... _AttributeExtensions>
void OTAProxy<_AttributeExtensions...>::CancelDownloadSetRequest(const ::v0::org::volvo::connectivity::OTATypes::UUID &_software, CommonAPI::CallStatus &_internalCallStatus, ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors &_error, ::v0::org::volvo::connectivity::OTATypes::GenericOkStatus &_status, const CommonAPI::CallInfo *_info) {
    delegate_->CancelDownloadSetRequest(_software, _internalCallStatus, _error, _status, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> OTAProxy<_AttributeExtensions...>::CancelDownloadSetRequestAsync(const ::v0::org::volvo::connectivity::OTATypes::UUID &_software, CancelDownloadSetRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->CancelDownloadSetRequestAsync(_software, _callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &OTAProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool OTAProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool OTAProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& OTAProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& OTAProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace connectivity
} // namespace volvo
} // namespace org
} // namespace v0



// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_VOLVO_CONNECTIVITY_OTA_PROXY_HPP_
