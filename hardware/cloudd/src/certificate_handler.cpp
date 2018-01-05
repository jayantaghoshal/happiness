#include "certificate_handler.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <sys/socket.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define LOG_TAG "CloudD.CertHandler"
#include <cutils/log.h>

namespace Connectivity {
const std::string& CA_CERT_PEM() {
    static const std::string ret =
            "-----BEGIN CERTIFICATE-----\n"
            "MIIF7zCCA9egAwIBAgIIH2jWyvuCgtQwDQYJKoZIhvcNAQELBQAweTEoMCYGA1UE\n"
            "AwwfQ29ubmVjdGVkIENhciBSb290IENBIHYxLjIgVEVTVDESMBAGA1UEBwwJVG9y\n"
            "c2xhbmRhMR0wGwYKCZImiZPyLGQBGRYNQ29ubmVjdGVkIENhcjEaMBgGCgmSJomT\n"
            "8ixkARkWClZvbHZvIENhcnMwIBcNMTMwNzMxMTE1ODI2WhgPMjA3MzA3MzExMTU4\n"
            "MjZaMHkxKDAmBgNVBAMMH0Nvbm5lY3RlZCBDYXIgUm9vdCBDQSB2MS4yIFRFU1Qx\n"
            "EjAQBgNVBAcMCVRvcnNsYW5kYTEdMBsGCgmSJomT8ixkARkWDUNvbm5lY3RlZCBD\n"
            "YXIxGjAYBgoJkiaJk/IsZAEZFgpWb2x2byBDYXJzMIICIjANBgkqhkiG9w0BAQEF\n"
            "AAOCAg8AMIICCgKCAgEAteRC/Bn1i2qXUfZdbWQEwFqL4BztPVD0HWxj0rG1Qh/i\n"
            "ZUhffLm6uHoKVgdxH+pwFXnbASXPHFGhNQaRfGuahJo6wF0niilcKFMn8D5Zgs7J\n"
            "7JLHCFhksJ5FvGQaG6N25ZqpD9aNs88PMPmSPgBnGdvPO/8pLYmpQ9EVkxmlXmRP\n"
            "rHUg6cykDocMci4TjgxUzDjztjqGlm5c/CB/W9FBwbGtgjqyizZAl2z5btKDeu7w\n"
            "dEvdoaRMD02F3WSKkb5Xq4aUx3zQTe7sYOmCPEElDhTGHEazX4nHCurJUJOXVr1k\n"
            "YGUttykfYctXh8bW+p9w8Cdll0B1OFPfPwX8Y4DOqaJ6mp8UPwy7uqJ+kkZ/v0uX\n"
            "Rps+sOmbCD91mTlMMrGk463oe9/Q3blW4oiwvJM26VCMNIXtCr/G+lYq6X+0CsW7\n"
            "mUoo395Z+6/sm4XVRywEzi9wf94jTLGahydc0pPYYsKErn7RmQ7lfXGzg8gxXgXm\n"
            "/E9dkxIHYF+iiDhNVs+sY37jZJtqruZj9wcXPGYhBZtO9BqC2pFhMyDaBOvBXXKd\n"
            "HSfNNJ49eCdmKvu15ZK5y222uZnTyk6cV+LqAHT6gOzUlXD4Pn1upVmRUObJpEpO\n"
            "A1affPV0k6HrMlQOKG0LvMTwu8Dj2nuUb+LQk+fCJUD4FMhWQCcNwY8vcjorKO8C\n"
            "AwEAAaN5MHcwHQYDVR0OBBYEFNVhg9MpkhseRwntX2m0QLptEyeqMA8GA1UdEwEB\n"
            "/wQFMAMBAf8wHwYDVR0jBBgwFoAU1WGD0ymSGx5HCe1fabRAum0TJ6owFAYDVR0g\n"
            "AQH/BAowCDAGBgRVHSAAMA4GA1UdDwEB/wQEAwIBBjANBgkqhkiG9w0BAQsFAAOC\n"
            "AgEAIlTQSCao/4J4UrQ2+AMXm7Rbgo1PLoOT9egu3kPItO1Tc8DgWXJoSwpGo2Vx\n"
            "ZI1h7YVGoNLDIx+W9UXLEQnLY3vD5a79stMK1hmg7dflPSs9YxI8j0qPAxJmRTaC\n"
            "rWMGls7c4KrYxcDnfFMOHbf6GSvg+sdxMteAfF2YH/VHfpsPIlugE3xaabOlkHPC\n"
            "3SSrDC+ltUJm1iCRCkCIQB6OL2CgtPCeuapwMYBr0KvbUjlG38ZrbtqO5s3oY0Uq\n"
            "1CzPWsE1zCHpixiQfoTuIgnCsZnQutB8l+4aE1n0hq6OQ/L+3Tal3yhsLBeRj96T\n"
            "8S0r2eXaQo9qxYD2K2JkTVryLH0EaSKYOCW9/S7mO1oDj4jJTKLgmEwfB2bGoM5B\n"
            "hWj+0Y/AUYqTqKHVsbAMukp7LIDJwd8RTDvt/11oYL30DNxxACYkc45sV6Lnvbtz\n"
            "TfN2QgRQHHyHXxSaHl1f1If1qDHtq3R0H1ZGn6Deen/g+FKaPTKsUVf9MXsWGddj\n"
            "aM5SXBHu8VLMEoQ6GXQ8+NrBQqtAwknwLvs8xmSP64dOuGVDDcjwjQ5WOuOm6e6h\n"
            "GoG4s0+V3x6k2iH6ZbrG5Oy3cRANnTUpRYNl6njq28g9ZyQ6aHm13a3dH9wLY24T\n"
            "/tHLoBELAKTLFMHZez8EDzDKyuE97EDNhrzuFtBHjagJ/Nw=\n"
            "-----END CERTIFICATE-----\n";
    return ret;
}

const std::string& CLIENT_CERT_PEM() {
    static const std::string ret =
            "-----BEGIN CERTIFICATE-----\n"
            "MIIFwjCCA6qgAwIBAgIIaKyy+mDcj8IwDQYJKoZIhvcNAQELBQAwbzEeMBwGA1UE\n"
            "AwwVVmVoaWNsZSBDQSB2MjAxMyBURVNUMRIwEAYDVQQHDAlUb3JzbGFuZGExHTAb\n"
            "BgoJkiaJk/IsZAEZFg1Db25uZWN0ZWQgQ2FyMRowGAYKCZImiZPyLGQBGRYKVm9s\n"
            "dm8gQ2FyczAeFw0xNDExMDQxNTA0MzNaFw00OTExMDQxNTA0MzNaMIGsMRMwEQYK\n"
            "CZImiZPyLGQBAQwDSUhVMRowGAYDVQQDDBEwMDAwMDAwMDAwMEtPREFQQTEMMAoG\n"
            "A1UEKQwDMjc1MQ8wDQYDVQQLDAYyMDE1MDUxDTALBgNVBAoMBDIwMTUxEDAOBgNV\n"
            "BAcMBzNTRTAwMTAxHTAbBgoJkiaJk/IsZAEZFg1Db25uZWN0ZWQgQ2FyMRowGAYK\n"
            "CZImiZPyLGQBGRYKVm9sdm8gQ2FyczCCASIwDQYJKoZIhvcNAQEBBQADggEPADCC\n"
            "AQoCggEBAIOs6ne1Bep9lsLAtFg90/eko3VzllL07309jVvsMZvM2NzYFPOdnWkV\n"
            "qTF4bcoi66wZZRDy7DT41iww/p0OsHBHTsrNWTbeH5nnSOw7x82NKsNTH527NQdL\n"
            "jAzGEJIxMncytyuVGSuXIxfLTdlu2DCoTNYtO4BPOLX3M0yZxP+u3tM7McTE7/uE\n"
            "d51pWLrgH/cKRFHJXVHY9Qjhns+dVPzqrqiB+9eoQ7xCqbZ8PvVwdYCFHmyJg/F1\n"
            "vXhbmdhn/i2VMILYaXOpWy4GSih/ksKJb2HPyAcv4B+vhpGSLwjb53vMzz9a/7Ts\n"
            "APMKVxJif4N9egRsXMQAPU9ZOWZyo9kCAwEAAaOCASIwggEeMIGIBggrBgEFBQcB\n"
            "AQR8MHowSAYIKwYBBQUHMAKGPGh0dHA6Ly90cnVzdC50ZXN0LnZvbHZvY2Fycy5j\n"
            "b20vY2MvdG9yc2xhbmRhL3ZlaGljbGUyMDEzLmNydDAuBggrBgEFBQcwAYYiaHR0\n"
            "cDovL29jc3AudGVzdC52b2x2b2NhcnMuY29tL2NjLzAdBgNVHQ4EFgQU6eYhHeMN\n"
            "4BjRXJxpIGnffFr/pbkwDAYDVR0TAQH/BAIwADAbBgNVHSABAf8EETAPMA0GCysG\n"
            "AQQBgqgcAgIBMB8GA1UdIwQYMBaAFP0J+8bT4Th6tw2fUrXgMbmh5E0eMBYGA1Ud\n"
            "JQEB/wQMMAoGCCsGAQUFBwMCMA4GA1UdDwEB/wQEAwIFoDANBgkqhkiG9w0BAQsF\n"
            "AAOCAgEAeRjwGLXDtd+AUF0WDuwMhna6c6ST0R+q1bOk6dgfdTGVJ6qiZyxi/mqH\n"
            "d7ooATVpFV+m1mxsq0KHIFwGteXaRC9g9F2eePkmkRB7lb26NND5HnzD9Z7Y7z9I\n"
            "N2sSMD33R5QJpmV31xyh6dg+nDHYJn3Ay4A+ni+8S+sb3MPDYT+90CMIocSIYia4\n"
            "u4qr1ZahjeFoHse/qgw+VOvOBWDTsnjW5YOXVDIGdBOL5eqRylZbL/LQJryc6ow6\n"
            "FcKwIFY4tFvWHNOlue7GumDO4scidMcp80PYhN7Iy3k/P6zYpVjizaC1+F268Uo3\n"
            "iAB0MNR1A2JEgXeiyv027/FjmXBErYW2d9lCKk9tqNm+4c3QS4lWd5t/mbzOyiJI\n"
            "Axay+KCN1Jek2PbmyVg5ZHim7j08lXHqRLph3QvY86ilznWAlyJ96BWdZuOS7kNZ\n"
            "0NhJwmaCx1csZQ0bLNZiN9YK7B9UxEWSVSwFFntjRRvuwbXBtSM/G+bVVZ6RZ0zk\n"
            "u2fiQgfmZyEYzkm4p/lKP8oN1Ky7I5jZgSRWlNlWGMWvj/Z050HP164wjyQZhRQi\n"
            "ByqjPlp9m0TWCtVt6qKS+qBTFjtDsJGBb1TIGFEc5ZyVzP1TyzfBFOQQYjeMzH5S\n"
            "OORXGH8xN+zEh5Re0aWf3dHz2jiUh1KAHNlWbwkMDlyddiQsPhI=\n"
            "-----END CERTIFICATE-----\n";
    return ret;
}
const std::string& CLIENT_KEY_PEM() {
    static const std::string ret =
            "-----BEGIN RSA PRIVATE KEY-----\n"
            "MIIEpAIBAAKCAQEAg6zqd7UF6n2WwsC0WD3T96SjdXOWUvTvfT2NW+wxm8zY3NgU\n"
            "852daRWpMXhtyiLrrBllEPLsNPjWLDD+nQ6wcEdOys1ZNt4fmedI7DvHzY0qw1Mf\n"
            "nbs1B0uMDMYQkjEydzK3K5UZK5cjF8tN2W7YMKhM1i07gE84tfczTJnE/67e0zsx\n"
            "xMTv+4R3nWlYuuAf9wpEUcldUdj1COGez51U/OquqIH716hDvEKptnw+9XB1gIUe\n"
            "bImD8XW9eFuZ2Gf+LZUwgthpc6lbLgZKKH+SwolvYc/IBy/gH6+GkZIvCNvne8zP\n"
            "P1r/tOwA8wpXEmJ/g316BGxcxAA9T1k5ZnKj2QIDAQABAoIBAAwPIAIKkBojmlUH\n"
            "tPyH3QvHBjlRdqjuSaicpH+QqBJutkWzfGe7PZlMJIylTNjATfka8kiVFtDbITI2\n"
            "4nqbx8jgnvSvzcXaneZIakNITkoi7ZGp4TFe3cJnRtI8qXPaEZcdC+1v4Dg/xTT+\n"
            "p4iMQDxkjXTn+5UKrFTt6MLRKluIFJ4pihYyyQNK4aGE6ysb8bRgZC6Ux+TUwQNc\n"
            "dNMYcKPahIpT21z4fyjxPYCyTkIglH/1ldExq3825DCXhRmbP+nYr518K73QYhtd\n"
            "Q2BQW7zpIFCXhfzfa7UZ32CBKTnm6YO/w1TOWBG/OnreWaxBEjpeQBi7itPrPt8M\n"
            "Oe3+84kCgYEAx0IUkS3vKneXP/jl8CqExi95qvWaX+5qJyATYuyFxfzrPpWYfe89\n"
            "EPEABRzltX4t6OlvHLBvj1ARhrCq1XWfOiXnu4K2mPWYTax0nlJuai1Eshp5nu2y\n"
            "4HzPCBub8ScQDfkK/UQ0X7zuQiMwdLcKt0UJxeeDXoXGjVZSyFK/TRMCgYEAqSwO\n"
            "0Q22f4/fsyWkOLTnulS9h+qaBvWIUTBQUOQlpSqvqTEtqePReTHHnUQ0vKFdZLrT\n"
            "OPqm2Y0ShG32TcOXI4ZUa/NuR9WrX2jbg4FJB/FGkFXCju/Mpgs5BSQhrGROj8d1\n"
            "V0nQ2yqbYQ7FTXlWpxWSDWKeoEKSoG9L1pSPBOMCgYBpyli3F8cyKdpguZoDH9/s\n"
            "hQo4zTsQEbnEWhBQ5jjD/Fs9mBklS56FXx7emBARgxOG5GD8v2QLl7wmYafhuxg1\n"
            "vChDcsvVfBdHrfcgk61RrAUb9lh6G8Nr0nfs2xO5Pj8U3BOMkeiop9tru9kfLot+\n"
            "8CWXMCqcRbQ8GAfNmbk9hwKBgQCLchKghcIp8ek+HUf5qjzc3Z8BGwNtuxRrf/yk\n"
            "QPs64y26tBvkzzJKyRq2apNh7U0zi8AyVoIUD3H1EUMWpKO/l6mc9iKwOdMyKRfQ\n"
            "lVAiqOI2dUqTUd0J/XQ55aeQF3qej5YMpn7vOXaskWVXUC56OZCTUfhZN6bvce48\n"
            "aWZeawKBgQClRn14yCOWwBBqh4nM7sweAoJpWYaLDkh4Il/lSmovGyncMs8swLf+\n"
            "CoVM/uzEv5jmMIh8MWAcrqdYT6+VSwtPl+RcSnERTbWh6IafIA8Ehj7uTS3sP70V\n"
            "E/cVV1To30TZXectPKEv25cBM5X/i+9sFETCGjeBCCdnzyNqdj0Iyw==\n"
            "-----END RSA PRIVATE KEY-----\n"
            "\n";
    return ret;
}

using BIOUnqPtr = std::unique_ptr<BIO, int (*)(BIO*)>;
X509UnqPtr X509Null() { return X509UnqPtr(nullptr, X509_free); }
EVP_PKEYUnqPtr EVP_PKeyNull() { return EVP_PKEYUnqPtr(nullptr, EVP_PKEY_free); }

X509UnqPtr CertFromPem(std::string pem_data) {
    BIOUnqPtr cert_bio(BIO_new(BIO_s_mem()), BIO_free);
    if (nullptr == cert_bio) {
        ALOGE("Could not create new BIO buffer");
        return X509Null();
    }

    // Write the cert data to the BIO buffer
    if (BIO_write(cert_bio.get(), static_cast<void*>(&pem_data[0]), pem_data.size()) <= 0) {
        ALOGE("Could not write data to BIO structure");
        return X509Null();
    }

    // Read the cert from BIO
    X509UnqPtr cert = X509UnqPtr(PEM_read_bio_X509_AUX(cert_bio.get(), NULL, NULL, NULL), X509_free);
    if (nullptr == cert) {
        ALOGE("Could not load certificate from BIO");
        return X509Null();
    }
    return cert;
}

EVP_PKEYUnqPtr KeyFromPem(std::string pem_data) {
    BIOUnqPtr key_data(BIO_new(BIO_s_mem()), BIO_free);
    if (nullptr == key_data) {
        ALOGE("Could not create new mem buffer");
        return EVP_PKeyNull();
    }

    // Insert the key data in the BIO structure
    if (BIO_write(key_data.get(), static_cast<void*>(&pem_data[0]), pem_data.size()) <= 0) {
        ALOGE("Could not write to BIO");
        return EVP_PKeyNull();
    }

    // Read the key from the BIO
    EVP_PKEYUnqPtr private_key =
            EVP_PKEYUnqPtr(PEM_read_bio_PrivateKey(key_data.get(), nullptr, nullptr, nullptr), EVP_PKEY_free);
    if (nullptr == private_key) {
        ALOGE("Could not read memory as EC private key");
        return EVP_PKeyNull();
    }

    return private_key;
}

CertHandler::CertHandler(const std::string& client_cert_pem, const std::string& client_key_pem,
                         const std::string& ca_cert)
    : m_ca_cert{X509Null()}, m_client_cert{X509Null()}, m_client_key{EVP_PKeyNull()} {
    // Set up the static cert
    if (!ca_cert.empty()) {
        m_ca_cert = CertFromPem(ca_cert);
        if (nullptr == m_ca_cert) {
            ALOGE("Could not load CA cert");
        } else {
            ALOGV("CA cert set up");
        }
    } else {
        ALOGW("No CA cert data available");
    }

    m_client_cert = CertFromPem(client_cert_pem);
    if (nullptr == m_client_cert) {
        ALOGE("Could not load client cert");
    } else {
        ALOGV("Client cert set up");
    }

    // Set up the static key
    m_client_key = KeyFromPem(client_key_pem);
    if (nullptr == m_client_key) {
        ALOGE("Could not load client key");
    } else {
        ALOGV("Client key set up");
    }
}

X509* CertHandler::GetCaCert() { return m_ca_cert.get(); }

X509* CertHandler::GetClientCert() { return m_client_cert.get(); }

EVP_PKEY* CertHandler::GetClientKey() { return m_client_key.get(); }

CertificateValidationStatus CertHandler::OnCreateOpenSslContext(void* ssl_ctx) noexcept {
    SSL_CTX* ctx = static_cast<SSL_CTX*>(ssl_ctx);
    X509* ca_cert = GetCaCert();
    X509* client_cert = GetClientCert();
    EVP_PKEY* client_key = GetClientKey();

    if (nullptr == client_cert) {
        ALOGE("Could not fetch client cert");
        return CertificateValidationStatus::Error;
    }
    if (nullptr == client_key) {
        ALOGE("Could not fetch client key");
        return CertificateValidationStatus::Error;
    }

    if (nullptr == ca_cert) {
        ALOGW("No CA certificate available");
        return CertificateValidationStatus::Error;
    } else {
        ALOGD("Setting CA cert");

        // better to use https://www.openssl.org/docs/man1.0.2/ssl/SSL_CTX_load_verify_locations.html if CA is in a file
        // instead of hard coded?
        X509_STORE* store = SSL_CTX_get_cert_store(ctx);
        if (X509_STORE_add_cert(store, ca_cert) == 0) {
            ALOGE("Could not add CA-certificate");
            return CertificateValidationStatus::Error;
        }
    }

    // https://wiki.openssl.org/index.php/Manual:SSL_CTX_use_certificate(3)
    //   "The SSL_CTX_* class of functions loads the certificates and keys into the SSL_CTX object ctx.
    //    The information is passed to SSL objects ssl created from ctx with SSL_new(3) by copying,
    //    so that changes applied to ctx do not propagate to already existing SSL objects."
    if (!SSL_CTX_use_certificate(ctx, client_cert)) {
        ALOGE("Could not assign client cert to TLS layer");
        return CertificateValidationStatus::Error;
    }

    if (!SSL_CTX_use_PrivateKey(ctx, client_key)) {
        ALOGE("Could not assign client key to TLS layer");
        return CertificateValidationStatus::Error;
    }

    return CertificateValidationStatus::Validated;
}

OcspRetCode CertHandler::GetOcspUrlFromServerCert(const std::string& server_url, std::string* ocsp) const {
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();

    auto ctx_deleter = [](SSL_CTX* ctx) {
        if (ctx) {
            SSL_CTX_free(ctx);
        }
    };

    std::unique_ptr<SSL_CTX, decltype(ctx_deleter)> ctx_handle(SSL_CTX_new(SSLv3_server_method()), ctx_deleter);
    if (!ctx_handle) {
        ALOGE("new SSL_CTX object failed (%s)", ERR_error_string(ERR_get_error(), nullptr));
        return OcspRetCode::NetworkFailure;
    }

    Socket sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_url.c_str(), "https", &hints, &servinfo)) != 0) {
        ALOGE("Unable to identify internet host for OCSP, error code %d", rv);
        return OcspRetCode::NetworkFailure;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd.sockfd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(sockfd.sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd.sockfd_);
            sockfd.sockfd_ = -1;
            continue;
        }

        break;  // if we get here, we must have connected successfully
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        // looped off the end of the list with no connection
        ALOGE("Failed to connect to %s for OCSP URI", server_url.c_str());
        return OcspRetCode::NetworkFailure;
    }

    auto ssl_deleter = [](SSL* ssl) {
        if (ssl) {
            SSL_free(ssl);
        }
    };

    std::unique_ptr<SSL, decltype(ssl_deleter)> ssl_handle(SSL_new(ctx_handle.get()), ssl_deleter);
    if (ssl_handle.get()) {
        int ret_set_fd = SSL_set_fd(ssl_handle.get(), sockfd.sockfd_);
        if (ret_set_fd == 0) {
            ALOGE("SSL set failed for OCSP socket fd (%s)", ERR_error_string(ERR_get_error(), nullptr));
            return OcspRetCode::NetworkFailure;
        }

        int err = SSL_connect(ssl_handle.get());
        ALOGW("Typically returns error code 0 and is still is connected. This is not according to SSL documentation.");
        // Typically returns error code 0 and is still is connected. This is not according to SSL documentation.
        if (err >= 0) {
            X509* server_cert = SSL_get_peer_certificate(ssl_handle.get());
            if (!server_cert) {
                ALOGE("Unable to get certificate from peer (%s)", ERR_error_string(ERR_get_error(), nullptr));
                return OcspRetCode::NetworkFailure;
            }
            stack_st_ACCESS_DESCRIPTION* values = reinterpret_cast<stack_st_ACCESS_DESCRIPTION*>(
                    X509_get_ext_d2i(server_cert, NID_info_access, NULL, NULL));
            if (!values) {
                ALOGE("Unable to extract values from certificate");
                X509_free(server_cert);
                return OcspRetCode::OcspFailure;
            }
            int size = sk_ACCESS_DESCRIPTION_num(values);

            for (int j = 0; j < size; j++) {
                ACCESS_DESCRIPTION* value = sk_ACCESS_DESCRIPTION_value(values, j);

                if (value && OBJ_obj2nid(value->method) == NID_ad_OCSP && value->location->type == GEN_URI) {
                    ocsp->assign(reinterpret_cast<char*>(value->location->d.uniformResourceIdentifier->data));
                    AUTHORITY_INFO_ACCESS_free(values);
                    X509_free(server_cert);
                    return OcspRetCode::Ok;
                }
            }

            AUTHORITY_INFO_ACCESS_free(values);
            X509_free(server_cert);
        } else {
            ALOGE("Unable to SSL connect to VCC sensus server (ret code %d) (%s)", err,
                  ERR_error_string(ERR_get_error(), nullptr));
            return OcspRetCode::Ok;
        }
    }

    ALOGE("SSL new failed for OCSP (%s)", ERR_error_string(ERR_get_error(), nullptr));
    return OcspRetCode::NetworkFailure;
}
}
