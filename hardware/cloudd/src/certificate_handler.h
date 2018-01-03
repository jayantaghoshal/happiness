/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <unistd.h>
#include <memory>
#include <string>
#include "certificate_handler_interface.h"
#include "openssl/err.h"

namespace Connectivity {
using X509UnqPtr = std::unique_ptr<X509, void (*)(X509*)>;
using EVP_PKEYUnqPtr = std::unique_ptr<EVP_PKEY, void (*)(EVP_PKEY*)>;

class Socket {
  public:
    ~Socket() {
        if (sockfd_ != -1) {
            close(sockfd_);
        }
    }

    int sockfd_ = -1;
};

/*
 * Contains and handles the certificates needed for a cloud connection. Can also validate incomming certificates.
 */
class CertHandler final : public CertHandlerInterface {
  public:
    CertHandler(const std::string& client_cert_pem, const std::string& client_key_pem, const std::string& ca_cert);

    CertificateValidationStatus OnCreateOpenSslContext(void* ssl_ctx) noexcept override;

    OcspRetCode GetOcspUrlFromServerCert(const std::string& server_url, std::string* ocsp) const;

  private:
    X509* GetCaCert();
    X509* GetClientCert();
    EVP_PKEY* GetClientKey();

    X509UnqPtr m_ca_cert;
    X509UnqPtr m_client_cert;
    EVP_PKEYUnqPtr m_client_key;
};
}