/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include "assert.h"
#include "gen_dataelements.h"

/**
 * Contains the stub for data elements
 */

namespace ApplicationDataElement {

/**
 * @brief The DEMsg class contains the content of one message
 */
class DEMsg {
  public:
    DEMsg(const char* data, uint32_t size, const std::string& name) {
        std::copy(&data[0], &data[size], back_inserter(data_));
        name_ = name;
    }

    std::vector<char> data_;
    std::string name_;
};

/**
 * @brief Contains all messages sent from this unit test
 */
class DEMsgContainer {
  public:
    static DEMsgContainer& instance() {
        static DEMsgContainer msgContainer;
        return msgContainer;
    }
    void addMsg(DEMsg msg) { deMsgs_.push_back(msg); }

    ///
    /// TODO add possibiltiy to check that a message is contained in the message
    /// queue.
    /// So that the unit tests can test if they send out correct values, eg:
    /// template<t> msgExistsByName(typename S::data_elem_type dataElem);
    /// template<t> msgExistsByContent(typename S::data_elem_type dataElem);
    /// template<t> lastMsgExistsByName(typename S::data_elem_type dataElem);
    /// template<t> lastMsgExistsByContent(typename S::data_elem_type dataElem);
    /// template<t> int msgCount(typename S::data_elem_type dataElem);

    std::vector<DEMsg> deMsgs_;
};

/**
 * The class that is used by the production code to send messages. In the stub
 * it adds a message to DEMsgContainer.
 */
template <typename S>
class DESender {
    static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for data elements that are out.");

  public:
    // send out a value for the data element represented by this instance
    void send(const typename S::data_elem_type& deValue) {
        DEMsg msg(&deValue, sizeof(deValue), S::Name());
        DEMsgContainer::instance().addMsg(msg);
    }
};

class DataElemValueBase {
  public:
    DataElemValueBase() {}
    virtual ~DataElemValueBase() {}

    enum class State {
        OK,    // the signal value is ok and available
        ERROR  // some kind of error state for the signal, like timeout or other
               // conditions (TODO what??).
    };

    //    virtual std::string name() const = 0;
    //    virtual autosar::Dir direction() const = 0;

    // State check convenience
    bool isOk() const { return (_state == State::OK); }
    bool isError() const { return (_state == State::ERROR); }

    long long timestamp() const;
    State state() const;
    int errorCode() const;

    DataElemValueBase(long long timestamp, State state, int errorCode) {
        _timestamp = timestamp;
        _state = state;
        _errorCode = errorCode;
    }

    // Signal content and state etc
    long long _timestamp;
    State _state;
    int _errorCode;
};

template <typename S>
class DataElemValue : public DataElemValueBase {
  public:
    using value_type = typename S::data_elem_type;

    static DataElemValue OK(const value_type& value, long long timestamp = -1) {
        return DataElemValue(value, timestamp, State::OK, 0);
    }
    static DataElemValue ERROR(int errorCode, long long timestamp = -1) {
        return DataElemValue(value_type(), timestamp, State::ERROR, errorCode);
    }
    virtual ~DataElemValue() {}

    value_type value() const { return m_value; }

    DataElemValue() {}
    DataElemValue(const value_type& value, long long timestamp, State state, int errorCode)
        : DataElemValueBase(timestamp, state, errorCode), m_value(value) {}

    // Signal content
    value_type m_value;
};

// ================================================
// DEReceiver
// class used to receive (listen+get) data elements from flexray
template <typename S>
class DEReceiver {
  public:
    DEReceiver() {
        _name = S::Name();

        if (deReceiverMap().find(_name) == deReceiverMap().end()) {
            deReceiverMap()[_name] = this;
        } else {
            assert("Several DEReceivers for the same signal name is not supported by "
                   "this stub");
        }
    }

    virtual ~DEReceiver() { deReceiverMap().erase(_name); }

    void subscribe(const std::function<void()>& callback) {
        if (deReceiverMap().find(_name) != deReceiverMap().end()) {
            deReceiverMap()[_name]->_callback = callback;
        }
    }

    DataElemValue<S> get() const { return _value; }

    /// Below is stub specific impl
    static std::map<std::string, DEReceiver<S>*>& deReceiverMap() {
        // Define a static map that is used for all DE Receiver objects
        static std::map<std::string, DEReceiver<S>*> deReceiverMap;
        return deReceiverMap;
    }

    void injectValue(const typename S::data_elem_type& deValue, long long timestamp) {
        _value.m_value = deValue;
        _value._state = DataElemValue<S>::State::OK;
        _value._timestamp = timestamp;
        _callback();
    }

    void injectError(int errorCode, long long timestamp) {
        _value._errorCode = errorCode;
        _value._state = DataElemValue<S>::State::ERROR;
        _callback();
    }

  private:
    std::string _name;
    std::function<void()> _callback;
    DataElemValue<S> _value;
};

// DEInjector
// class used by unit tests to inject a value
template <typename S>
class DEInjector {
  public:
    void inject(const typename S::data_elem_type& deValue, long long timestamp = -1) {
        if (DEReceiver<S>::deReceiverMap().find(S::Name()) != DEReceiver<S>::deReceiverMap().end()) {
            DEReceiver<S>::deReceiverMap()[S::Name()]->injectValue(deValue, timestamp);
        }
    }

    // Inject a data element error

    void error(int errorCode, long long timestamp = -1) {
        if (DEReceiver<S>::deReceiverMap().find(S::Name()) != DEReceiver<S>::deReceiverMap().end()) {
            DEReceiver<S>::deReceiverMap()[S::Name()]->injectError(errorCode, timestamp);
        }
    }
};

}  // end of namespace
