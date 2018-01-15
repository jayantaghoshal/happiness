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
/*
 * Class responsible for fetching and parsing the URL of a Cloud Entry Point(CEP) from the (Central Node Entry Point)
 */
class EntryPointFetcher final {
  public:
    EntryPointFetcher(std::shared_ptr<CertHandlerInterface> cert_handler,
                      std::shared_ptr<CloudRequestHandlerInterface> cloud_request_handler,
                      const bool allow_retry = true);

    /*
     * Configures a request to start fetching the CEP URL. Will call the when_result_available_callback_ when done.
     */
    void Fetch(const std::string& entry_point_url) throw(std::runtime_error);

    void Stop();

    /*
     * Sets the callback method that will be called when the URL of the CEP is fetched. NOTE: the Entry Point URL is
     * empty if the fetching or parsing failed.
     */
    void WhenResultAvailable(std::function<void(const EntryPointParser::EntryPoint&)>&& callback);

    static EntryPointParser::EntryPoint parse(const char* const data);

  private:
    void Restart() throw(std::runtime_error);
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