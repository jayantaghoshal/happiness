#include "vipcom_client.h"
#include <ivi-logging.h>
#include <unistd.h>

// Define a context for the IVI-logging library to use.
typedef logging::DefaultLogContext LogContext;
LOG_DECLARE_CLASS_CONTEXT("CCVP", "Updating values for VIP");

CarConfigVipCom::CarConfigVipCom() : vipCom(VipCom::Application::systemIpcIndex, this) {
  vipResponded = false;
  vipAcknowledge = INVALID_VIP_REPLY;
}

int CarConfigVipCom::sendConfig(std::vector<uint8_t> &values) {
  log_info() << "Sending CarConfig values to VIP";
  // Prepend the control byte.
  values.insert(values.begin(), hisipBytes::sysSetCarConfigControlByte);
  // Send the message to VIP.
  return vipCom.sendMessage(hisipBytes::sysSetCarConfigFid, values);
}

int CarConfigVipCom::waitForVipAcknowledge() {
  // Wait for VIP response.
  log_debug() << "Waiting for VIP response...";
  int timeouts = 0;
  // Check "vipResponded" member for async response from VipCom library.
  while (!vipResponded) {
    usleep(TIMEOUT_USEC);
    timeouts++;
    // Stop and fail if reaching the allowed number of timeouts.
    if (timeouts == TIMEOUT_COUNT) {
      log_error() << "No response from VIP in expected time";
      return -1;
    }
  }

  // Verify VIP response based on HISIP catalog:
  if (vipAcknowledge == hisipBytes::sysRepCarConfigAck) {
    log_debug() << "Received ACK from VIP";
    return 0;
  } else if (vipAcknowledge == hisipBytes::sysRepCarConfigNack) {
    log_error() << "Received NACK from VIP";
    return -1;
  } else {
    log_error() << "Received unexpected response from VIP: " << vipAcknowledge;
    return -1;
  }
}

void CarConfigVipCom::onMessage(const uint8_t &_fid, const std::vector<uint8_t> &_payload) {
  // Accept messages with correct FID.
  if (_fid == hisipBytes::sysRepCarConfigFid) {
    // Grab payload and break the waiting loop through "vipResponded" member.
    vipAcknowledge = _payload.at(0);
    vipResponded = true;
  } else {
    log_warn() << "Received a message with unexpected FID: " << _fid;
  }
}

void CarConfigVipCom::onConnectionError(ConnectionError e) {
  if (IVipComClient::ConnectionError::CONNECT_FAILED == e) {
    // Set invalid payload and break the waiting loop through "vipResponded" member.
    vipAcknowledge = INVALID_VIP_REPLY;
    vipResponded = true;
    log_error() << "VipCom error with initial connection";
  } else {
    log_error() << "VipCom error with existing connection";
  }
}
