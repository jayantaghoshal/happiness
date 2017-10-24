/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus.h"
#include <mosquittopp.h>

#include <cutils/log.h>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <map>
#include <set>

#undef LOG_TAG
#define LOG_TAG "DataElementDBus"

LOG_SET_DEFAULT_CONTEXT(deleContext)

namespace {
int isInitialized = 0;

std::mutex mutex_isInitialized;

const std::string BROKER_ADDRESS = "127.0.0.1";

std::string initMosquitto() {
  int tmpInt;
  {
    std::lock_guard<std::mutex> safeLock(mutex_isInitialized);  // mutex_isInitialized is locked here
    if (0 == isInitialized) {
      mosqpp::lib_init();
    }
    tmpInt = isInitialized++;

    // mutex_isInitialized is unlocked when leaving this scope
  }

  int major = -1, minor = -1, rev = -1;
  mosqpp::lib_version(&major, &minor, &rev);
  const std::string name = "DataElementCommBus[" + std::to_string(getpid()) + "-" + std::to_string(tmpInt) + "]";
  log_debug() << name << ": mosquitto initialized (" << major << "," << minor << "," << rev << ")";

  return name;
}

void deinitMosquitto() {
  std::lock_guard<std::mutex> safeLock(mutex_isInitialized);  // mutex_isInitialized is locked here
  if (isInitialized > 0) {
    isInitialized--;
    if (0 == isInitialized) {
      mosqpp::lib_cleanup();
    }
  } else {
    log_warn() << "deinitMosquitto() called more times than initMosquitto()";
  }
  // mutex_isInitialized is unlocked when leaving this scope
}

std::string busName(const std::string& name, autosar::Dir dir) {
  std::string r;

  switch (dir) {
    case autosar::Dir::IN:
      r = "/i/" + name;
      break;

    case autosar::Dir::OUT:
      r = "/o/" + name;
      break;

    case autosar::Dir::INTERNAL:
      r = "/s/" + name;
      break;
  }
  return r;
}

template<typename T>
const T readInMutex(T& valueToRead, std::mutex& m) {
    std::lock_guard<std::mutex> lock(m);
    const T tmp = valueToRead;
    return tmp;
}

} // end of anonymous namespace

class DataElementCommBus : public IDataElementCommBus, mosqpp::mosquittopp {
 public:
  DataElementCommBus(const std::string brokerAddress);

  void setNewDataElementHandler(std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) override;
  void addName(autosar::Dir dir, const std::string& name) override;
  void send(const std::string& name, const std::string& payload, autosar::Dir dir) override;
  //void setCallback(autosar::Dir dir, std::function<void(const std::string& name, const QByteArray& payload)>&& newDataElementCallback) override;
  virtual ~DataElementCommBus();

 private:
  // mosquitto overrides
  void on_connect(int rc) override;
  void on_disconnect(int rc) override;
  void on_log(int level, const char* str) override;
  void on_error() override;
  void on_message(const struct mosquitto_message* message) override;

  std::function<void(const std::string& name, const std::string& payload)> _callback;

  // Mutex required because written by DEReceiver and read by mosquitto connect-callback
  std::mutex pendingMessageMutex;
  std::mutex pendingSubscriptionMutex;
  std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessages;
  std::set<std::tuple<std::string, autosar::Dir>> pendingSubscriptions;
};

IDataElementCommBus* IDataElementCommBus::create() {
    //The address should be fetched from some kind of config or similar ...
    log_info() << "DataElementCommBus::create()";
    return new DataElementCommBus(BROKER_ADDRESS);
}

DataElementCommBus::DataElementCommBus(const std::string brokerAddress) : mosquittopp(initMosquitto().c_str(), false) {
  // Starting mosquitto sending worker thread/run loop

  log_info() << "DataElementCommBus connect_async";

  int connectRetCode = connect_async(brokerAddress.c_str());  // connect to the localhost Mosquitto broker.
  if (connectRetCode != MOSQ_ERR_SUCCESS && connectRetCode != MOSQ_ERR_ERRNO)
  {
      log_warning() << "DataElementCommBus error in connect_async: "<< mosquitto_strerror(connectRetCode) << " (" << connectRetCode << ") " << brokerAddress;
      // lets try a few more times
      for (int i=0 ; i<10 ; i++) {
          usleep(200000);
          log_info() << "DataElementCommBus retrying connect_async";
          connectRetCode = connect_async(brokerAddress.c_str());  // connect to the localhost Mosquitto broker.

          // The return codes MOSQ_ERR_SUCCESS and MOSQ_ERR_ERRNO means that the coonnection will later on be performed async.
          // This is not obvious from the mosquitto documentation but by reading the source code of the mosuitto lib.
          // According to documentation for example MOSQ_ERR_EAI shall never be returned but it happens and in which case
          // the async connection does not work (therfor the re-try).
          if (connectRetCode != MOSQ_ERR_SUCCESS && connectRetCode != MOSQ_ERR_ERRNO) {
              log_warning() << "DataElementCommBus retry-error in connect_async: "<< mosquitto_strerror(connectRetCode) << " (" << connectRetCode << ") " << brokerAddress;
          } else {
              break; // success
          }
      }
      if (connectRetCode != MOSQ_ERR_SUCCESS && connectRetCode != MOSQ_ERR_ERRNO) {
          log_fatal() << "DataElementCommBus failed all re-tries, bailing out.";
          return;
      }
  }
  int loopStartRetCode = loop_start();                   // start the mosquitto run loop thread
  if (loopStartRetCode != MOSQ_ERR_SUCCESS)
  {
     log_fatal() << "DataElementCommBus error in loop_start, code: " << mosquitto_strerror(loopStartRetCode) << " (" << loopStartRetCode << ")";
  }
}

DataElementCommBus::~DataElementCommBus() {
  disconnect();
  loop_stop();
  deinitMosquitto();
}

//TODO: Add return code to this function to propagate errors all the way up to application
void DataElementCommBus::send(const std::string& name, const std::string& payload, autosar::Dir dir) {
  const std::string busname = busName(name, dir);

  {
      auto key = std::make_tuple(name, dir);
      std::lock_guard<std::mutex> lock(pendingMessageMutex);
      pendingSendMessages[key] = payload;
  }

  const int retCode = publish(NULL, busname.c_str(), payload.size(), payload.data(), 0 /*qos*/, true /*retain*/);
  if (retCode != MOSQ_ERR_SUCCESS)
  {
      log_warn() << "DataElementCommBus::send, publish returned error, code: " << mosquitto_strerror(retCode) << " (" << retCode << "). Name: " << name;
  }
}

void DataElementCommBus::setNewDataElementHandler(std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) {
    _callback = std::move(newDataElementCallback);
}

void DataElementCommBus::addName(autosar::Dir dir, const std::string& name) {
    const std::string topic = busName(name,dir);
    log_debug() << "DataElementCommBus::Subscribing on topic:" << topic;

    {
        std::lock_guard<std::mutex> lock(pendingSubscriptionMutex);
        pendingSubscriptions.emplace(name, dir);
    }

    const int retCode = subscribe(nullptr, topic.c_str());
    if (retCode != MOSQ_ERR_SUCCESS)
    {
        log_warn() << "DataElementCommBus::addName, subscribe returned error: " << mosquitto_strerror(retCode)
                   << " (" << retCode << "). Name: " << name;
    }
}

// Mosquittopp
void DataElementCommBus::on_connect(int rc) {
    if (rc != 0)
    {
        log_warn() << "ERROR: DataElementCommBus::on_connect: " << mosquitto_connack_string(rc) << " (" << rc << ")";
    }
    else
    {
        log_info() << "DataElementCommBus::on_connect SUCCESS";

        const auto tmpPendingSubReadInMutex = readInMutex(pendingSubscriptions, pendingSubscriptionMutex);
        const auto tmpPendingMessagesReadInMutex = readInMutex(pendingSendMessages, pendingMessageMutex);

        log_info() << "Retry total nr of " << tmpPendingSubReadInMutex.size() << " subscriptions";
        for (const auto& sub : tmpPendingSubReadInMutex)
        {
            const auto subName = std::get<0>(sub);
            const auto subDir  = std::get<1>(sub);
            log_verbose() << "Resubscribing: "<< subName;
            addName(subDir, subName);
        }

        log_info() << "Resend total nr of " << tmpPendingMessagesReadInMutex.size() << " messages";
        for (const auto& msg : tmpPendingMessagesReadInMutex)
        {
            auto msgName = std::get<0>(msg.first);
            auto msgDir  =std::get<1>(msg.first);
            auto payload = msg.second;
            log_verbose() << "Resending: " << msgName;
            send(msgName, payload, msgDir);
        }
    }
}

void DataElementCommBus::on_disconnect(int rc) {
    if (rc != 0)
    {
        log_warn() << "DataElementCommBus::on_disconnect, unexpected disconnect, reason: " << mosquitto_strerror(rc) << " (" << rc << ")";
    }
    else
    {
        log_verbose() << "DataElementCommBus::on_disconnect SUCCESS";
    }
}

void DataElementCommBus::on_log(int /*level*/, const char* /*str*/) {
  // log_verbose() << "DataElementCommBus::on_log["<<level<<"]: "<<str;
}

void DataElementCommBus::on_error() { log_warn() << "DataElementCommBus::on_error"; }

void DataElementCommBus::on_message(const struct mosquitto_message* message) {
  if (_callback) {
    std::string name = message->topic;

    // remove "/i/" or "/o/" or "/s/" from the name
    if (name.length() > 4) {
      name = name.substr(3, name.length() - 3);
      const std::string payload((char*)message->payload, message->payloadlen);
      try
      {
        _callback(name, payload);
      }
      catch (const std::exception &e)
      {
        log_error() << "unhandled exception in DataElementCommBus::on_message callback,"
                    << " topic: " << message->topic
                    << " error: " << e.what();
      }
    }
  }
}
