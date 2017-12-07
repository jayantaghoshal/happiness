#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CERTIFICATE_HANDLER_INTERFACE_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CERTIFICATE_HANDLER_INTERFACE_H

#include <string>

namespace Connectivity {
const std::string& CA_CERT_PEM();
const std::string& CLIENT_CERT_PEM();
const std::string& CLIENT_KEY_PEM();

enum class CertificateValidationStatus { Validated, Error };

enum class OcspRetCode : uint32_t { Ok = 0, NetworkFailure = 1, OcspFailure = 2 };

class ICertHandler {
  public:
    virtual ~ICertHandler() = default;
    virtual CertificateValidationStatus OnCreateOpenSslContext(void* ssl_ctx) noexcept = 0;
    virtual OcspRetCode GetOcspUrlFromServerCert(const std::string& server_url, std::string& ocsp) const = 0;
};
}
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CERTIFICATE_HANDLER_INTERFACE_H