#include "cloudService.h"
#include <curl/curl.h>

#define LOG_TAG "CloudD.service"

namespace Connectivity {

CloudService::CloudService() : eventDispatcher_{IDispatcher::GetDefaultDispatcher()} {}

bool CloudService::Initialize() { return true; }

void CloudService::ConnectionTest() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        ALOGI("Make request");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            ALOGD("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            ALOGD("Great success!");
        }

        curl_easy_cleanup(curl);
    }
}
}