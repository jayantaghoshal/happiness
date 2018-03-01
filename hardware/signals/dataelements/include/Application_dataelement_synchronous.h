/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "callbackwrapper.h"
#include "dataelementframework.h"
#include "jsonserializers.h"

namespace ApplicationDataElement {

class DataElementsExecutionContext {
  public:
    virtual void assertInExecutionContext() = 0;
    virtual void enqueueFunction(std::function<void()> func) = 0;
};

/*!
 * \brief DEReceiver, template class used to receive (get + subscribe) signals from flexray and LIN.
 *
 * The class is template instantiated with the classes declared in gen_dataelements.h
 * that inherits from the InTag or the InternalTag.
 */
template <typename S>
class DESynchronousReceiver {
  public:
    DESynchronousReceiver(DataElementsExecutionContext& context) : context_{context}, {
        receiver.subscribe([&]() {
            // TODO(performance): Inefficient copies?
            auto value = receiver.get() context.enqueueFunction([this, value]() {
                _value = value;
                performCallback();
            });
        });
    }

    virtual void performCallback() {
        // keep copy of callback in case _callback tries to unsubscribe itself while running
        auto cbcopy = _callback;
        cbcopy(_value);
    }

    /*!
     * \brief Subscribe to changes in the signal state and/or value.
     * \param callback A callback that will be called once the state and/or value
     * of the signal has changed. Example:
     * \code
     * DEReceiver<autosar::ADataRawSafe_info> myInSignal;
     * autosar::ADataRawSafe1 myValue = myInSIgnal.get().value();
     * myInSignal.subscribe([&]() {
     *    // do something with the in-signal
     *    auto v = myInSignal.get().value();
     *    :
     * });
     * \endcode
     */
    void subscribe(const std::function<void()>& callback) {
        context_.assertInExecutionContext();
        const bool hasValidValue = (_value.isOk() || (_value.isError() && _value.errorCode() != -1));
        _callback.set(callback);
        if (callback != nullptr && hasValidValue) {
            // performing direct callback since we have a valid value
            performCallback();
        }
    }

    /*!
     * \brief Subscribe to changes in the signal state and/or value.
     * \param callback A callback that will be called once the state and/or value
     * of the signal has changed. The callback contains the signalvalue.
     * If you use C++14 then use it like this:
     * \code
     * DEReceiver<autosar::ADataRawSafe_info> myInSignal;
     * myInSignal.subscribeVal([](auto signalValue) {
     *    // do something with the in-signal
     *    if ( signalValue.isOk() ) {
     *       :
     *    }
     *    :
     * });
     * \endcode
     *
     * If you use ancient C++11 use it like this:
     * \code
     * DEReceiver<autosar::ADataRawSafe_info> myInSignal;
     * myInSignal.subscribeVal([](DataElemValue<autosar::ADataRawSafe_info> signalValue) {
     *    // do something with the in-signal
     *    if ( signalValue.isOk() ) {
     *       :
     *    }
     *    :
     * });
     * \endcode
     */
    void subscribeVal(const std::function<void(const DataElemValue<S>&)>& callback) {
        context_.assertInExecutionContext();
        const bool hasValidValue = (_value.isOk() || (_value.isError() && _value.errorCode() != -1));
        _callback.set(callback);
        if (callback != nullptr && hasValidValue) {
            // performing direct callback since we have a valid value
            callback(_value);
        }
    }

    /*!
     * \brief Get signal information from the DEReceiver instance.
     * The value received from get() provides information about the signal state and also the real signal value.
     * Normally it is used something like this:
     * \code
     * auto v = myInSignal.get();
     * if (v.isOk())
     * {
     *    auto signalValue = v.value();
     * }
     * else
     * {
     *    auto errorCode = v.errorCode();
     * }
     * \endcode
     */
    DataElemValue<S> get() const {
        context_.assertInExecutionContext();
        return _value;
    }

  private:
    // These have no meaning really
    DESynchronousReceiver(const DEReceiver&) = delete;
    DESynchronousReceiver(const DEReceiver&&) = delete;
    DESynchronousReceiver& operator=(const DEReceiver&) = delete;
    DESynchronousReceiver& operator=(const DEReceiver&&) = delete;

    std::mutex valueMutex;
    CallbackWrapper<DataElemValue<S>> _callback;
    DataElemValue<S> _value;
    DEReceiver<S> receiver;
    DataElementsExecutionContext& context;
};

}  // end of namespace
