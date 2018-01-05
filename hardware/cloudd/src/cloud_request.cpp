#include "cloud_request.h"

namespace Connectivity {

bool CloudRequest::SetCurlHandle(CURL *curl_handle) {
    if (curl_handle_) {
        return false;
    }

    curl_handle_ = curl_handle;

    return true;
}

}  // namespace Connectivity