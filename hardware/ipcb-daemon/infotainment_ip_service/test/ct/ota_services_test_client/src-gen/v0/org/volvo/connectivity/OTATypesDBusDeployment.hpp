/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus 3.1.5.v201512091512.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V0_ORG_VOLVO_CONNECTIVITY_OTA_TYPES_DBUS_DEPLOYMENT_HPP_
#define V0_ORG_VOLVO_CONNECTIVITY_OTA_TYPES_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace volvo {
namespace connectivity {
namespace OTATypes_ {

// typecollection-specific deployment types
typedef CommonAPI::EmptyDeployment GenericOkStatusDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment
> UUIDDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment
> PartIdentifierDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment
> UriDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::DBus::ArrayDeployment<
        CommonAPI::DBus::StructDeployment<
            CommonAPI::DBus::StructDeployment<
                CommonAPI::DBus::StringDeployment
            >,
            CommonAPI::EmptyDeployment,
            CommonAPI::EmptyDeployment
        >
    >
> DownloadStatus_ResponseDeployment_t;

typedef CommonAPI::EmptyDeployment DownloadStatusDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> SoftwareProductDeployment_t;

typedef CommonAPI::EmptyDeployment DownloadStateDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >
> OpDownloadSoftware_SetRequestDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >
> OpDeletedSoftware_SetRequestDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >
> OpCancelDownload_SetRequestDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment
> OpCancelDownload_ResponseDeployment_t;

typedef CommonAPI::EmptyDeployment InstallProgressDeployment_t;

typedef CommonAPI::EmptyDeployment InstallCommandDeployment_t;

typedef CommonAPI::EmptyDeployment InstallStatusDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >,
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >,
    CommonAPI::EmptyDeployment
> InstallationDataDeployment_t;

typedef CommonAPI::EmptyDeployment FileInstallationStatusDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::DBus::StringDeployment
    >,
    CommonAPI::EmptyDeployment
> InstalledDataDeployment_t;


// typecollection-specific deployments

} // namespace OTATypes_
} // namespace connectivity
} // namespace volvo
} // namespace org
} // namespace v0

#endif // V0_ORG_VOLVO_CONNECTIVITY_OTA_TYPES_DBUS_DEPLOYMENT_HPP_
