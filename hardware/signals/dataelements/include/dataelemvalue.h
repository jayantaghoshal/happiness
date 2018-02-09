/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

/*!
 * \file
 *
 * \brief This file contains signal and signal value classes and enums
 */

#include <cutils/log.h>
#include "gen_dataelements.h"
#include "internalsignals.h"

/*!
 * \enum SignalErrorCode
 * \brief The SignalErrorCode enum provides valid errorCodes for DataElemValue/DataElemValueBase
 */
enum class SignalErrorCode {
    NEVER_RECEIVED = -1, /*!< The signal has never been received     */
    TIMEOUT = 0          /*!< There has been a timeout on the signal */
};

/*!
 * \brief DataElemValueBase, provides basic information about a signal.
 * DataElemValueBase is the base class to DataElemValue<S> and provides all information
 * that is not dependant on the template type S.
 *
 * The following information comes from DataElemValueBase:
 *    state (OK|ERROR)
 *    errorCode (see SignalErrorCode enum above)
 *    timestamp (in milliseconds since Jan 1, 1970 UTC - https://en.wikipedia.org/wiki/Unix_time)
 */
class DataElemValueBase {
  public:
    DataElemValueBase();
    virtual ~DataElemValueBase();

    enum class State {
        OK,    // the signal value is ok and available
        ERROR  // some kind of error state for the signal, like timeout or other conditions. See SignalErrorCode enum.
    };

    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& ba) = 0;
    virtual std::string name() const = 0;
    virtual autosar::Dir direction() const = 0;

    /*!
     * \brief isOk convenience function to check if signal state==ok
     * \return true when state==Ok, false otherwise
     */
    bool isOk() const;

    /*!
     * \brief isError convenience function to check if signal state==error
     * \return true when state==error, false otherwise
     */
    bool isError() const;

    /*!
     * \brief timestamp Creation timestamp of signal.
     * \return timestamp in milliseconds since Jan 1, 1970 UTC (https://en.wikipedia.org/wiki/Unix_time)
     */
    int64_t timestamp() const;

    /*!
     * \brief state Signal state
     * \return State enum
     */
    State state() const;

    /*!
     * \brief errorCode
     * \return The signal error code. Only valid if state==error. See SignalErrorCode above for valid values.
     */
    int errorCode() const;

#ifndef UNIT_TEST
  private:
#endif
    // Signal content and state etc
    int64_t _timestamp;
    State _state;
    int _errorCode;

  protected:
    DataElemValueBase(int64_t timestamp, State state, int errorCode);
};

// ====================================================
// Serializers - these are implemented else-where
template <typename S>
class DataElemValue;

// The impl of these two functions shall be in a file separated from DataElemValue
template <typename S>
std::string serialize(const DataElemValue<S>& dev);
template <typename S>
DataElemValue<S> deserialize(const std::string& ba);

/*!
 * \brief DataElemValue, provides basic information about a signal.
 * that is not dependant on the template type S.
 *
 * The following information comes from DataElemValue:
 *    name (a signal unique name)
 *    direction (in, out, internal)
 *    value (the signal value)
 */
template <typename S>
class DataElemValue : public DataElemValueBase {
    friend class DataElementFramework;

  public:
    using value_type = typename S::data_elem_type;

    static DataElemValue OK(const value_type& value, int64_t timestamp = -1) {
        return DataElemValue(value, timestamp, State::OK, 0);
    }
    static DataElemValue ERROR(int errorCode, int64_t timestamp = -1) {
        return DataElemValue(value_type(), timestamp, State::ERROR, errorCode);
    }
    virtual ~DataElemValue() {}

    virtual std::string serialize() const { return ::serialize(*this); }
    virtual void deserialize(const std::string& ba) { *this = ::deserialize<S>(ba); }
    /*!
     * \brief name
     * \return A unique signal name
     */
    virtual std::string name() const { return S::Name(); }
    /*!
     * \brief direction
     * \return Dir enum - valid directions are: in, out, internal (relative to the IHU/MP)
     */
    virtual autosar::Dir direction() const { return S::Direction(); }
    /*!
     * \brief value
     * \return the signal value. Only valid if state==ok. The type of value depends on the signal (S::data_elem_type).
     */
    value_type value() const {
        if (isError()) {
            ALOG(LOG_ERROR, "DataElementValue",
                 "DEReceiver, reading value() of signal that is in error state, signalname: %s", name().c_str());
        }
        return m_value;
    }

  private:
    DataElemValue() {}
    DataElemValue(const value_type& value, int64_t timestamp, State state, int errorCode)
        : DataElemValueBase(timestamp, state, errorCode), m_value(value) {}

    // Signal content
    value_type m_value;
};
