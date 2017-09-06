/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef APPLICATIONDATAELEMENT_H
#define APPLICATIONDATAELEMENT_H

/*!
 * \file
 *
 * \brief This is the key file for all application code in the MP to include when using the Dataelement Framework.
 *
 * It provides access to DEReceiver (used to receive flexray and LIN signals) and
 * DESender (used to send out flexray and LIN signals).
 */

#include "dataelementframework.h"
#include "jsonserializers.h"
#include "callbackwrapper.h"

namespace ApplicationDataElement {

/*!
 * \brief DESender, template class used to send out signals on flexray and LIN.
 *
 * The class is template instantiated with the classes declared in gen_dataelements.h
 * that inherits from the OutTag or the InternalTag.
 *
 * Example:
 * \code
 * DESender<autosar::AccAdprSpdLimActv_info> myOutSignal;
 * autosar::OnOffPen myValue;
 * myValue.Sts = autosar::OnOff1::Off;
 * myValue.Pen = autosar::IdPen::Prof4;
 * myOutSignal.send(myValue);
 * \endcode
 */
template<typename S>
class DESender {
static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for data elements that are out.");
public:
    /*!
     * \brief Send out a value for the signal represented by the DESender instance.
     *
     * \param deValue The value to send out. Its type is given from the DESender template class S::data_elem_type
     * Most of these types are declared in gen_datatypes.h but for simple types like bool, int etc the type is shown directly
     * in gen_dataelements.h
     */
    void send( const typename S::data_elem_type& deValue ) {
        DataElementFramework::instance().inject<S>(deValue,-1);
    }
};


/*!
 * \brief DEReceiver, template class used to receive (get + subscribe) signals from flexray and LIN.
 *
 * The class is template instantiated with the classes declared in gen_dataelements.h
 * that inherits from the InTag or the InternalTag.
 */
template<typename S>
class DEReceiver : public INewDataElement {
static_assert(std::is_base_of<autosar::InTag, S>::value, "Only for data elements that are out.");
public:
    DEReceiver() : _value(DataElemValue<S>::ERROR(-1)) {
        _value = DataElementFramework::instance().subscribe<S>(this);
    }

    virtual ~DEReceiver() {
        DataElementFramework::instance().unsubscribe(S::Name(), this);
    }

    virtual void newDataElementValue(const DataElemValueBase& newvalue) {
        // rtti not supported -> static_cast used instead of dynaic_cast.
        // Should be safe anyway since it is generatred code that calls this method
        _value = static_cast<const DataElemValue<S>&>(newvalue);
    }

    virtual void performCallback() {
        std::unique_lock<std::mutex> protectCallback(setCallbackMutex);
        _callback(_value);
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
    void subscribe( const std::function<void()>& callback ) {
        const bool hasValidValue = (_value.isOk() || (_value.isError() && _value.errorCode()!=-1));

        {
            std::unique_lock<std::mutex> protectCallback(setCallbackMutex);
            _callback.set(callback);
        }

        if ( callback != nullptr && hasValidValue) {
            // performing direct callback since we have a valid value
            callback();
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
    void subscribeVal( const std::function<void(const DataElemValue<S>&)>& callback ) {
        const bool hasValidValue = (_value.isOk() || (_value.isError() && _value.errorCode()!=-1));

        {
            std::unique_lock<std::mutex> protectCallback(setCallbackMutex);
            _callback.set(callback);
        }

        if ( callback != nullptr && hasValidValue) {
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
        return _value;
    }

private:
    // These have no meaning really
    DEReceiver(const DEReceiver&)=delete;
    DEReceiver(const DEReceiver&&)=delete;
    DEReceiver& operator=(const DEReceiver&)=delete;
    DEReceiver& operator=(const DEReceiver&&)=delete;

    std::mutex setCallbackMutex;
    CallbackWrapper<DataElemValue<S>> _callback;
    DataElemValue<S> _value;
};

} // end of namespace
#endif // APPLICATIONDATAELEMENT_H

