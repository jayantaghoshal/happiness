/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H

#include <IDispatcher.h>
#include <vendor/volvocars/hardware/http/1.0/IHttpRequest.h>
#include <list>
#include "certificate_handler_interface.h"
#include "cloud_request.h"
#include "cloudd_local_config.h"
#include "vpn_entry_point_fetcher.h"

using ::tarmac::eventloop::IDispatcher;
using ::vendor::volvocars::hardware::http::V1_0::HttpHeaderField;
using ::vendor::volvocars::hardware::http::V1_0::HttpHeaders;

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_string;
using ::vendor::volvocars::hardware::http::V1_0::IHttpRequest;
using ::vendor::volvocars::hardware::http::V1_0::Response;
namespace Connectivity {

class CloudService : public IHttpRequest {
  public:
    CloudService();
    ~CloudService() = default;

    bool Initialize();

  private:
    // Methods from IHttpRequest follow.
    Return<void> doGetRequest(const hidl_string& uri, const HttpHeaders& headers, bool use_https, uint32_t timeout,
                              doGetRequest_cb _hidl_cb);

    Response BuildResponse(std::int32_t code, const std::string& data, const std::string& header);
    IDispatcher& eventDispatcher_;

    ClouddLocalConfig cloudd_local_config_;

    CloudRequest cloud_request_;

    ICertHandler* certHandler_;

    VPNEntryPointFetcher entry_point_fetcher_;

    std::string cep_url_;
    int cep_port_;
};

}  // namespace Connectivity
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H