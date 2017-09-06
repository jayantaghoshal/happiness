/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef DATAELEMVALUE_H
#define DATAELEMVALUE_H

#include <QByteArray>
#include "gen_dataelements.h"
#include "internalsignals.h"

// ====================================================
// DataElemValueBase
class DataElemValueBase {
public:
    DataElemValueBase();
    virtual ~DataElemValueBase();

    enum class State {
        OK, // the signal value is ok and available
        ERROR // some kind of error state for the signal, like timeout or other conditions (TODO what??).
        //NOT_AVAILABLE // the signal is not available due to car config/local config, too early in startup etc   TODO this state shall probably be removed
    };

    virtual QByteArray serialize() const = 0;
    virtual void deserialize(const QByteArray& ba) = 0;
    virtual std::string name() const = 0;
    virtual autosar::Dir direction() const = 0;

    // State check convenience
    bool isOk() const;
    bool isError() const;

    long long timestamp() const;
    State state() const;
    int errorCode() const;

    //operator std::string() const; // to be able to print it out
    void setError(int errorCode); // Only for UT

protected:
    DataElemValueBase(long long timestamp, State state, int errorCode);

private:
    // Signal content and state etc
    long long _timestamp;
    State _state;
    int _errorCode;
};



// ====================================================
// Serializers - these are implemented else-where
template<typename S> class DataElemValue;

// The impl of these two functions shall be in a file separated from DataElemValue
template<typename S> QByteArray serialize(const DataElemValue<S>& dev);
template<typename S> DataElemValue<S> deserialize(const QByteArray& ba);



// ====================================================
// DataElemValue
template<typename S>
class DataElemValue : public DataElemValueBase {
friend class DataElementFramework;

public:
    using value_type = typename S::data_elem_type;

    static DataElemValue OK(const value_type& value, long long timestamp=-1) { return DataElemValue(value, timestamp, State::OK, 0); }
    static DataElemValue ERROR(int errorCode, long long timestamp=-1) { return DataElemValue(value_type(), timestamp, State::ERROR, errorCode); }
    virtual ~DataElemValue() {}

    virtual QByteArray serialize() const { return ::serialize(*this); }
    virtual void deserialize(const QByteArray &ba) { *this = ::deserialize<S>(ba); }
    virtual std::string name() const { return S::Name(); }
    virtual autosar::Dir direction() const { return S::Direction(); }

    value_type value() const { return m_value; }

private:
    DataElemValue() {}
    DataElemValue(const value_type& value, long long timestamp, State state, int errorCode) : DataElemValueBase(timestamp,state,errorCode), m_value(value) {}

    // Signal content
    value_type m_value;
};

#endif // DATAELEMVALUE_H
