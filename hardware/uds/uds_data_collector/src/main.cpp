#include <hidl/HidlTransportSupport.h>

#include "UdsDataCollector.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// TODO(krzysztof.wesolowski@volvocars.com) subscribe as DiagKeys handler,
// translate them back to DIDs and ask registered providers.
int main() {
    configureRpcThreadpool(1, true);
    auto data_collector = std::make_unique<UdsDataCollector>();

    auto* collector = static_cast<IUdsDataCollector*>(data_collector.get());
    auto* aggregated_provider = static_cast<IUdsDataProvider*>(data_collector.get());

    android::status_t status;
    status = collector->registerAsService();
    if (status != android::OK) {
      ALOGE("Could not register collector as service, %d", status);
    }

    status = aggregated_provider->registerAsService("test-point");
    if (status != android::OK) {
      ALOGE("Could not register aggregated_provider as service, %d", status);
    }

    joinRpcThreadpool();
}
