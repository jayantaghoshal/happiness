#pragma once

#include <curl/curl.h>

#include <functional>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include "certificate_handler_interface.h"
#include "cloud_request_handler_interface.h"
#include "entry_point_parser.h"

#include <IDispatcher.h>

using ::tarmac::eventloop::IDispatcher;

namespace Connectivity {

class EntryPointFetcher final {
  public:
    EntryPointFetcher(std::shared_ptr<CertHandlerInterface> cert_handler,
                      std::shared_ptr<CloudRequestHandlerInterface> cloud_request_handler,
                      const bool allow_retry = true);

    void Fetch(const std::string& entry_point_url);

    void Stop();

    void WhenResultAvailable(std::function<void(const EntryPointParser::EntryPoint&)>&& callback);

    static EntryPointParser::EntryPoint parse(const char* const data);

  private:
    void Restart();
    void worker();

    std::shared_ptr<CertHandlerInterface> cert_handler_;
    std::shared_ptr<CloudRequestHandlerInterface> cloud_request_handler_;
    std::string entry_point_url_;
    const bool allow_retry_;
    size_t retry_index_ = 0;
    IDispatcher::JobId retry_timer_handle_;

    std::function<void(const EntryPointParser::EntryPoint&)> when_result_available_callback_;
};

}  // namespace Connectivity