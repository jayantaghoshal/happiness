#ifndef NETMAN_DIAGNOSTICS_REPORTER_H
#define NETMAN_DIAGNOSTICS_REPORTER_H

#include <utils/StrongPointer.h>
#include <vcc/localconfig.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>

namespace vcc {
namespace netman {

class DiagnosticsReporter final : public ::android::hidl::manager::V1_0::IServiceNotification,
                                  public virtual ::android::RefBase,
                                  public ::android::hardware::hidl_death_recipient {
  public:
    explicit DiagnosticsReporter(const vcc::LocalConfigReaderInterface* lcfg = vcc::LocalConfigDefault());

  private:
    void check_link();

    ::android::sp<::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector> diag_service_;

    ::android::hardware::Return<void> onRegistration(const ::android::hardware::hidl_string& fqName,
                                                     const ::android::hardware::hidl_string& name,
                                                     bool preexisting) override;

    void serviceDied(uint64_t cookie, const ::android::wp<IBase>& who) override;
    int getDtcID(const std::string& iface);

    std::array<std::string, 3> ifnames_;
    const vcc::LocalConfigReaderInterface* const lcfg_;
};

}  // namespace netman
}  // namespace vcc

#endif  // NETMAN_DIAGNOSTICS_REPORTER_H
