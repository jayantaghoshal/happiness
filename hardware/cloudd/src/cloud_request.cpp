#include "cloud_request.h"
//#include <stdlib.h>

#define LOG_TAG "CloudD.CloudRequest"
#include <cutils/log.h>

namespace Connectivity {

namespace {
template <typename T>
void verified_curl_multi_setopt(CURLM* curl, const CURLMoption opt, T data) {
    const CURLMcode res = curl_multi_setopt(curl, opt, data);
    if (res != CURLMcode::CURLM_OK) {
        throw std::runtime_error("Failed to set curl multi option: " + std::to_string(opt) + ", error: " +
                                 std::to_string(res) + " " + curl_multi_strerror(res));
    }
}

template <typename T>
void verified_curl_easy_setopt(CURL* curl, const CURLoption opt, T data) {
    const CURLcode res = curl_easy_setopt(curl, opt, data);
    if (res != CURLcode::CURLE_OK) {
        throw std::runtime_error("Failed to set curl option: " + std::to_string(opt) + ", error: " +
                                 std::to_string(res) + " " + curl_easy_strerror(res));
    }
}

class RequestHandleData final : public ICloudRequest::IRequestHandleData {
  public:
    RequestHandleData(std::unique_ptr<CloudRequest::EasyState> easyState) : easyState{std::move(easyState)} {}

  private:
    // Not entirely unique, curl has a non-owning raw pointer to easyState through the "private userdata"
    // This is safe because when destroying the EasyState it will automatically remove itself from
    // curl.
    std::unique_ptr<CloudRequest::EasyState> easyState;
};

CURLcode OnCreateOpenSslContext(CURL* curl, void* ssl_ctx, void* user_param) noexcept {
    ALOGI(" + OnCreateOpenSslContext");
    using namespace Connectivity;
    (void)curl;
    CloudRequest::EasyState* userData = static_cast<CloudRequest::EasyState*>(user_param);
    ICertHandler& certHandler = *(userData->certHandler);

    CertificateValidationStatus certStatus = certHandler.OnCreateOpenSslContext(ssl_ctx);

    ALOGI(" - OnCreateOpenSslContext");
    if (certStatus == CertificateValidationStatus::Validated) {
        return CURLE_OK;
    } else {
        return CURLE_SSL_CERTPROBLEM;
    }
}
}

int perform(CloudRequest::MultiState* multi_state, curl_socket_t fd) {
    ALOGI(" + perform\nCurl Socket: %d", fd);
    int runningHandles = 0;

    int ret = curl_multi_socket_action(multi_state->curlm_.get(), fd, 0, &runningHandles);
    if (ret != CURLcode::CURLE_OK) {
        ALOGE("Unable to perform multi socket action.");
        return -1;
    }

    int msgs_queue_out = 0;

    do {
        CURLMsg* curl_message = curl_multi_info_read(multi_state->curlm_.get(), &msgs_queue_out);

        if (curl_message && curl_message->msg == CURLMSG_DONE) {
            ALOGI("Done %i", msgs_queue_out);

            std::int32_t response_code = -1;
            CloudRequest::EasyState* easy_state = nullptr;
            int ret1 = curl_easy_getinfo(curl_message->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
            int ret2 = curl_easy_getinfo(curl_message->easy_handle, CURLINFO_PRIVATE, &easy_state);

            // Remove the easy handle from multi and cleanup the easy-handle
            // NOTE: !!! We have to hack a bit with the finishedPrematurely flag because
            //          we might have been called from curl_multi_add_handle() here.
            //          To make sure that easy handle is destroyed AFTER easymulti-handle
            //          we set the finishedPrematurely-flag and we DON't free the easy-handle
            //          here, we let the caller of curl_multi_add_handle do so.

            easy_state->finishedPrematurely = (easy_state->multi_easy_connection == nullptr);
            easy_state->multi_easy_connection = nullptr;

            CurlEasyUnique keepEasyAliveUntilWeFinishThisFunction = nullptr;
            if (!easy_state->finishedPrematurely) {
                keepEasyAliveUntilWeFinishThisFunction.swap(easy_state->curle_);
            }

            const CURLcode resultCode = curl_message->data.result;

            if (ret1 != CURLcode::CURLE_OK) {
                ALOGE("Failed to get CURL response1: %d", ret1);
            } else if (ret2 != CURLcode::CURLE_OK) {
                ALOGE("Failed to get CURL response2: %d", ret2);
            } else if (resultCode == CURLcode::CURLE_OPERATION_TIMEDOUT) {
                ALOGW("Request timeout");
                easy_state->response_callback_(599, "", "");
            } else if (resultCode != CURLcode::CURLE_OK) {
                ALOGD("Request failed with error: %d, %s", resultCode, curl_easy_strerror(resultCode));
                easy_state->response_callback_(-1, "", "");
            } else {
                easy_state->response_callback_(response_code, easy_state->data_, easy_state->header_);
            }
        }

    } while (msgs_queue_out > 0);

    return 0;
}

int curl_socket_callback(CURL* easy, curl_socket_t fd, int action, void* userdata, void* socket) {
    (void)easy;
    (void)socket;

    if (action == CURL_POLL_REMOVE) {
        IDispatcher::GetDefaultDispatcher().RemoveFd(fd);
    } else {
        std::uint32_t events = 0;
        if (action == CURL_POLL_IN || action == CURL_POLL_INOUT) events |= EPOLLIN;
        if (action == CURL_POLL_OUT || action == CURL_POLL_INOUT) events |= EPOLLOUT;

        IDispatcher::GetDefaultDispatcher().AddFd(fd,
                                                  [userdata, fd]() {
                                                      CloudRequest::MultiState* multi_state =
                                                              static_cast<CloudRequest::MultiState*>(userdata);
                                                      return perform(multi_state, fd);
                                                  },
                                                  events);
    }

    return 0;
}

int curl_timer_callback(CURLM* multi, long timeout_ms, void* userdata) {
    (void)multi;

    CloudRequest::MultiState* multi_state = static_cast<CloudRequest::MultiState*>(userdata);

    // Curl is requesting perform right away
    if (timeout_ms == 0) {
        perform(multi_state, CURL_SOCKET_TIMEOUT);
    }
    // Curl is requesting a timeout
    else if (timeout_ms > 0) {
        multi_state->timer_job_id = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
                std::chrono::milliseconds(timeout_ms), [userdata]() {
                    CloudRequest::MultiState* multi_state = static_cast<CloudRequest::MultiState*>(userdata);
                    return perform(multi_state, CURL_SOCKET_TIMEOUT);
                });
    }
    // Curl wants to cancel timer.
    else {
        IDispatcher::GetDefaultDispatcher().Cancel(multi_state->timer_job_id);
    }

    return 0;
}

size_t curl_write_callback(char* ptr, const size_t size, const size_t nmemb, void* userdata) {
    const size_t real_size = size * nmemb;

    std::string* write_buffer = static_cast<std::string*>(userdata);

    try {
        write_buffer->append(ptr, real_size);
    } catch (...) {
        ALOGE("Error writing to CURL receive buffer.");
        return 0;
    }

    return real_size;
}

const char* infoTypeStr(const curl_infotype type) {
    switch (type) {
        case CURLINFO_TEXT:
            return " TEXT => ";
        case CURLINFO_HEADER_OUT:
            return " HEADER => ";
        case CURLINFO_DATA_OUT:
            return " DATA => ";
        case CURLINFO_SSL_DATA_OUT:
            return " SSL =>";
        case CURLINFO_HEADER_IN:
            return " HEADER <=";
        case CURLINFO_DATA_IN:
            return " DATA <=";
        case CURLINFO_SSL_DATA_IN:
            return " SSL <=";
        default:
            return "??? -";
    }
}

int debug_callback(CURL* const handle, const curl_infotype type, char* const data, const size_t size,
                   void* const userptr) {
    (void)handle;
    (void)userptr;

    std::string logString = "";

    if (data != nullptr) {
        logString = std::string{data, size};
    }
    ALOGI("CURL: %s %s", infoTypeStr(type), logString.c_str());
    return 0;
}

CloudRequest::CloudRequest() { curl_global_init(CURL_GLOBAL_ALL); }

bool CloudRequest::Init() {
    ALOGI(" + Init");

    if (multi_state_.curlm_ == nullptr) {
        multi_state_.curlm_.reset(curl_multi_init());
        if (multi_state_.curlm_ == nullptr) {
            ALOGE("Failed to initialize CURL multi.");
            return false;
        }

        try {
            verified_curl_multi_setopt(multi_state_.curlm_.get(), CURLMOPT_SOCKETDATA, &multi_state_);
            verified_curl_multi_setopt(multi_state_.curlm_.get(), CURLMOPT_SOCKETFUNCTION, curl_socket_callback);
            verified_curl_multi_setopt(multi_state_.curlm_.get(), CURLMOPT_TIMERDATA, &multi_state_);
            verified_curl_multi_setopt(multi_state_.curlm_.get(), CURLMOPT_TIMERFUNCTION, curl_timer_callback);
        } catch (std::exception& e) {
            ALOGE("%s", e.what());
            return false;
        }
    }

    ALOGI(" - Init");
    return true;
}

CloudRequest::RequestHandle CloudRequest::GenericGet(const std::string& uri, CloudResponseCallback&& response_callback,
                                                     std::function<void(CURL* curl, EasyState& easyState)> setOptFunc,
                                                     ICertHandler* certHandler, const RequestConfig& config) {
    ALOGI(" + GenericGet");
    std::unique_ptr<EasyState> easy_state = std::make_unique<EasyState>();
    easy_state->response_callback_ = std::move(response_callback);

    CurlEasyUnique easy_unique{curl_easy_init()};
    CURL* easy = easy_unique.get();
    if (easy == nullptr) {
        ALOGE("Failed to initalize CURL easy.");
        throw std::runtime_error("Failed to initalize CURL easy");
    }

    easy_state->curle_ = std::move(easy_unique);
    easy_state->certHandler = certHandler;

    try {
        char const* const verboseEnv = getenv("NWH_CLOUDREQUEST_VERBOSE");
        const bool verbose = (verboseEnv != nullptr && (strcmp("0", verboseEnv) != 0));
        verified_curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose);
        verified_curl_easy_setopt(easy, CURLOPT_DEBUGFUNCTION, debug_callback);
        verified_curl_easy_setopt(easy, CURLOPT_URL, uri.c_str());
        verified_curl_easy_setopt(easy, CURLOPT_HTTPGET, 1);
        verified_curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);

        verified_curl_easy_setopt(easy, CURLOPT_WRITEDATA, &easy_state->data_);
        verified_curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, curl_write_callback);
        verified_curl_easy_setopt(easy, CURLOPT_HEADERDATA, &easy_state->header_);
        verified_curl_easy_setopt(easy, CURLOPT_PRIVATE, easy_state.get());
        verified_curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, config.timeout.count());

        if (!config.header_list.empty()) {
            struct curl_slist* chunk = NULL;
            for (auto p : config.header_list) {
                chunk = curl_slist_append(chunk, p.c_str());
            }
            verified_curl_easy_setopt(easy, CURLOPT_HTTPHEADER, chunk);
        }

        setOptFunc(easy, *easy_state);

    } catch (std::exception& e) {
        ALOGE("%s", e.what());
        throw std::runtime_error("Failed to set curl options");
    }

    int res = curl_easy_perform(easy);
    /*
    if (res != CURLE_OK) {
        ALOGD("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        ALOGD("Great success!");
    }
    */

    std::int32_t response_code = -1;
    CloudRequest::EasyState* easy_st = nullptr;
    int ret1 = curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &response_code);
    int ret2 = curl_easy_getinfo(easy, CURLINFO_PRIVATE, &easy_state);

    // Remove the easy handle from multi and cleanup the easy-handle
    // NOTE: !!! We have to hack a bit with the finishedPrematurely flag because
    //          we might have been called from curl_multi_add_handle() here.
    //          To make sure that easy handle is destroyed AFTER easymulti-handle
    //          we set the finishedPrematurely-flag and we DON't free the easy-handle
    //          here, we let the caller of curl_multi_add_handle do so.

    /*
        easy_state->finishedPrematurely = (easy_state->multi_easy_connection == nullptr);
        easy_state->multi_easy_connection = nullptr;


        CurlEasyUnique keepEasyAliveUntilWeFinishThisFunction = nullptr;
        if (!easy_state->finishedPrematurely) {
            keepEasyAliveUntilWeFinishThisFunction.swap(easy_state->curle_);
        }

    */
    if (ret1 != CURLcode::CURLE_OK) {
        ALOGE("Failed to get CURL response1: %d", ret1);
    } else if (ret2 != CURLcode::CURLE_OK) {
        ALOGE("Failed to get CURL response2: %d", ret2);
    } else {
        easy_state->response_callback_(response_code, easy_state->data_, easy_state->header_);
    }

    curl_easy_cleanup(easy);

    /*
        try {
            // NOTE!!!  curl_multi_add_handle() might SYNCHRONOUSLY perform some actions which might eventually end up
            //          in perform() with the operation completed/failed.
            //          Due to this, the multi_easy_connection could have a valid handle that should not be
            //          This is undocumented but observed on target.
            //          This is probably related to https://curl.haxx.se/libcurl/c/libcurl-multi.html#BLOCKING

            easy_state->multi_easy_connection = std::make_unique<CurlMultiEasyHandle>(multi_state_.curlm_.get(), easy);

            if (easy_state->finishedPrematurely) {
                ALOGD("Request finished prematurely");
                easy_state->multi_easy_connection = nullptr;
                easy_state->curle_ = nullptr;
            }
        } catch (std::exception& e) {
            ALOGE("Unable to add easy to multi handle. %s", e.what());
            throw std::runtime_error("Failed to add easy handle to multi");
        }

        int runningHandles = 0;
        CURLMcode code = curl_multi_socket_action(multi_state_.curlm_.get(), CURL_SOCKET_TIMEOUT, 0, &runningHandles);
        if (code != CURLMcode::CURLM_OK) {
            ALOGE("Unable to perform multi socket action to start http get.");
            throw std::runtime_error("Unable to perform multi socket action to start http get.");
        }
    */
    ALOGI(" - GenericGet");
    return std::make_unique<RequestHandleData>(std::move(easy_state));
}

CloudRequest::RequestHandle CloudRequest::HttpGet(const std::string& uri, const RequestConfig& config,
                                                  CloudResponseCallback&& response_callback) {
    ALOGI(" + HttpGet");
    const auto setOpts = [&](CURL* easy, EasyState& easyState) {
        (void)easy;
        (void)easyState;
    };
    ALOGI(" - HttpGet");
    return GenericGet(uri, std::move(response_callback), setOpts, nullptr, config);
}

CloudRequest::RequestHandle CloudRequest::HttpsGet(const std::string& uri, const RequestConfig& config,
                                                   CloudResponseCallback&& response_callback,
                                                   ICertHandler* certHandler) {
    ALOGI(" + HttpsGet");

    const auto setOpts = [&](CURL* easy, EasyState& easyState) {

        verified_curl_easy_setopt(easy, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
        verified_curl_easy_setopt(easy, CURLOPT_SSL_CTX_DATA, &easyState);
        verified_curl_easy_setopt(easy, CURLOPT_SSL_CTX_FUNCTION, OnCreateOpenSslContext);
        verified_curl_easy_setopt(easy, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(easy, CURLOPT_SSL_CIPHER_LIST, "ECDHE-RSA-AES128-SHA");

    };
    ALOGI(" - HttpsGet");
    return GenericGet(uri, std::move(response_callback), setOpts, certHandler, config);
}
}
