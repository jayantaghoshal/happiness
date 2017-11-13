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

    collector->registerAsService();
    aggregated_provider->registerAsService("test-point");

    joinRpcThreadpool();
}
