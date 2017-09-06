/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef APPLICATIONDATAELEMENT_H
#define APPLICATIONDATAELEMENT_H

#include "dataelementframework.h"
#include "jsonserializers.h"

namespace ApplicationDataElement {

// =======================================================
// This is the key file for all application code in the MP
// to include when using the Data Element Framework
// =======================================================


// ================================================
// DESender
// class used to send a data element out on flexray
template<typename S>
class DESender {
static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for data elements that are out.");
public:
    // send out a value for the data element represented by this instance
    void send( const typename S::data_elem_type& deValue ) {
        DataElementFramework::instance().inject<S>(deValue,-1);
    }
};


// ================================================
// DEReceiver
// class used to receive (listen+get) data elements from flexray
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
        _value = dynamic_cast<const DataElemValue<S>&>(newvalue);
        if (this->_callback) _callback();
    }

    void subscribe( const std::function<void()>& callback ) {
        _callback = callback;
        if ( this->_callback && (_value.isOk() || (_value.isError() && _value.errorCode()!=-1)) ) {
            // performing direct callback since we have a valid value
            //fprintf(stderr,"Direct callback for %s\n",S::Name());
            _callback();
        }
    }

    DataElemValue<S> get() const {
        return _value;
    }

private:
    // These have no meaning really
    DEReceiver(const DEReceiver&)=delete;
    DEReceiver(const DEReceiver&&)=delete;
    DEReceiver& operator=(const DEReceiver&)=delete;
    DEReceiver& operator=(const DEReceiver&&)=delete;

    std::function<void()> _callback;
    DataElemValue<S> _value;
};

} // end of namespace
#endif // APPLICATIONDATAELEMENT_H

