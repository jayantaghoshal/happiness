/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <vendor/volvocars/hardware/cloud/1.0/ICloudConnection.h>
#include <vendor/volvocars/hardware/cloud/1.0/ICloudConnectionDownloadResponseCallback.h>
#include <vendor/volvocars/hardware/cloud/1.0/ICloudConnectionEventListener.h>
#include <list>
#include "certificate_handler_interface.h"
#include "cloud_request_handler.h"
#include "cloudd_local_config.h"
#include "entry_point_fetcher.h"

#include "mqtt_callbacks.h"
using ::tarmac::eventloop::IDispatcher;
using ::vendor::volvocars::hardware::cloud::V1_0::HttpHeaderField;
using ::vendor::volvocars::hardware::cloud::V1_0::HttpHeaders;

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_string;
using ::vendor::volvocars::hardware::cloud::V1_0::ICloudConnection;
using ::vendor::volvocars::hardware::cloud::V1_0::ICloudConnectionEventListener;
using ::vendor::volvocars::hardware::cloud::V1_0::ICloudConnectionDownloadResponseCallback;
using ::vendor::volvocars::hardware::cloud::V1_0::Response;

namespace Connectivity {

/*
 * Controller class for the cloud daemon. Implements the HAL interface and owns the URI to CEP
 */
class CloudService : public ICloudConnection {
  public:
    enum ConnectionState { INIT, CONNECTION, CONNECTED, NOCONNECTION };

    CloudService();
    ~CloudService() = default;

    /*
     * Initializes the service. Register HAL services and start fetching the CEP.
     * @return false if failed to register HAL or setting up the fetch request, else true
     */
    bool Initialize();

  private:
    /*
     * Start fetching the URI for CEP. Forces exit if making the request fails. If the requests succeeds, the cep_url_
     * and cep_port_ will be populated.
     */
    bool FetchEntryPoint();

    // Methods from ICloudConnection follow.
    Return<void> registerCloudConnectionEventListener(const android::sp<ICloudConnectionEventListener>& listener);

    Return<void> doGetRequest(const hidl_string& uri,
                              const HttpHeaders& headers,
                              uint32_t timeout,
                              doGetRequest_cb _hidl_cb);

    Return<void> doPostRequest(const hidl_string& uri,
                               const HttpHeaders& headers,
                               const hidl_string& body,
                               uint32_t timeout,
                               doPostRequest_cb _hidl_cb);

    Return<void> downloadRequest(const hidl_string& uri,
                                 const HttpHeaders& headers,
                                 const hidl_string& file_path,
                                 uint32_t timeout,
                                 const android::sp<ICloudConnectionDownloadResponseCallback>& callback);

    /*
     * Helper method to create a parseable Response out of data received from curl after a successful request.
     * @parm code HTTP status code
     * @parm data Data received from curl request
     * @parm header Header received from curl request
     * @return A Response containing the code, data and a parsed header list that can be sent over Binder.
     */
    Response BuildResponse(std::int32_t code, const std::string& data, const std::string& header);

    void SetConnectionState(const ConnectionState state);

    ConnectionState state_;
    std::vector<android::sp<ICloudConnectionEventListener> > listeners_;

    ClouddLocalConfig cloudd_local_config_;
    std::shared_ptr<CloudRequestHandler> cloud_request_handler_;
    std::shared_ptr<CertHandlerInterface> cert_handler_;
    EntryPointFetcher entry_point_fetcher_;

    // MQTT client handler
    mqtt::async_client_ptr client_;
    mqtt::connect_options connopts_;
    MqttCallback mqtt_cb_;
    void ConnectToMqttServer();

    std::shared_ptr<CloudRequest> download_request_;

    std::string cep_url_;
    int cep_port_;
    std::string client_uri_;
    std::string cep_mqtt_server_;
};

}  // namespace Connectivity