/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef ECDDATAELEMENT_H
#define ECDDATAELEMENT_H

#include "dataelementframework.h"
#include "jsonserializers.h"

namespace ECDDataElement {

// =======================================================
// This is the key file for all ECD!!! code in the MP
// to include when using the Data Element Framework
// =======================================================


// ================================================
// DEInjector
// class used to inject a, from flexray, received Data Element into the "virtual" bus in the MP
template<typename S>
class DEInjector {
static_assert(std::is_base_of<autosar::InTag, S>::value, "Only for IN Data Elements");
public:
    // Inject a data element value, received from flexray, towards application code. Used by ECD code.
    // timestamp is milliseconds since Jan 1, 1970 UTC (https://en.wikipedia.org/wiki/Unix_time)
    void inject( const typename S::data_elem_type& deValue, long long timestamp=-1 ) {
        DataElementFramework::instance().inject<S>(deValue, timestamp);
    }

    // Inject a data element error (due to timeout!?) towards application code. Used by ECD code.
    // timestamp is milliseconds since Jan 1, 1970 UTC (https://en.wikipedia.org/wiki/Unix_time)
    void error( int errorCode, long long timestamp=-1 ) {
        DataElementFramework::instance().injectError<S>(errorCode, timestamp);
    }
};


// ================================================
// DESink
// class used to sink (receive) Data Elements (that has been sent out by application code)
// and send it further to the VIP & flexray
template<typename S>
class DESink : public INewDataElement {
static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for OUT Data Elements");
public:
    DESink() : _value(DataElementFramework::instance().subscribe<S>(this)) {
        //DataElementFramework::instance().sinkSubscribe<S>(this);
    }

    virtual ~DESink() {
        DataElementFramework::instance().unsubscribe(S::Name(), this);
    }

    virtual void newDataElementValue(const DataElemValueBase& newvalue) {
        _value = dynamic_cast<const DataElemValue<S>&>(newvalue);
        if (this->_callback) _callback();
    }

    void subscribe( const std::function<void()>& callback ) {
        _callback = callback;
        if ( this->_callback && (_value.isOk() || (_value.isError() && _value.errorCode()!=-1)) ) {
            // performing direct callback since we have a valid value
            _callback();
        }
    }

    DataElemValue<S> get() const {
        return _value;
    }

private:
    // These have no meaning really
    DESink(const DESink&)=delete;
    DESink(const DESink&&)=delete;
    DESink& operator=(const DESink&)=delete;
    DESink& operator=(const DESink&&)=delete;

    std::function<void()> _callback;
    DataElemValue<S> _value;
};

} // end of namespace
#endif // ECDDATAELEMENT_H

