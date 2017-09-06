/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef DATAELEMENTFRAMEWORK_H
#define DATAELEMENTFRAMEWORK_H

#include <string>
#include <map>
#include <list>
#include <type_traits>

#include <functional>
#include <memory>
#include <mutex>

#include "dataelemvalue.h"
#include "gen_dataelements.h"
#include "internalsignals.h"

#include "dataelementcommbus.h"

// ===============================================
// callback interface to deliver new DataElemValue values
class INewDataElement {
public:
    virtual void newDataElementValue(const DataElemValueBase& newvalue)=0;
};


// ===============================================
// Data element framework to send and receive data elements
// Normally an application does not use this interface directly but instead one of the wrappers/helpers available in
// DataElementSender and DataElementReceiver below
class DataElementFramework {
public:
    // Obtain the instance to the singleton object
    static DataElementFramework& instance();

    // ============================================================
    // methods used by application code
    // Send a data element "downstream" out on flexray. Used by the application code.
/*
    template<typename S> void send( const typename S::data_elem_type& deValue ) {
        static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for out data elements"); // Make sure we only use this for NOT IN data elements

        auto dev = DataElemValue<S>::OK(deValue);
        _inject(dev);
    }
    */

    // Listen to a data-element from Flexray. Used by the application code.
    template<typename S> DataElemValue<S> subscribe( INewDataElement* callback ) {
        std::unique_ptr<DataElemValueBase> dev(new DataElemValue<S>(DataElemValue<S>::ERROR(-1)));
        subscribe(dev, callback);

        return get<S>();
    }

    // Get the latest value for an IN data element. Used by the application code.
    template<typename S> DataElemValue<S> get() {
        auto v = get(S::Name());
        if (v==nullptr) {
            // This is the first time we access this data element so lets add it
            subscribe<S>(nullptr);
            v = get(S::Name());
        }

        return *dynamic_cast<const DataElemValue<S>*>(v);
    }


    // ============================================================
    // methods used by ECD code
    // Inject a data element value, received from flexray, towards application code. Used by ECD code.
    template<typename S> void inject( const typename S::data_elem_type& deValue, long long timestamp ) {
        //static_assert(std::is_base_of<autosar::InTag, S>::value, "Only for IN data elements"); // Make sure we only use this for IN data elements

        auto dev = DataElemValue<S>::OK(deValue, timestamp);
        _inject(dev);
    }

    // Inject a data element error, due to timeout (!!??), towards application code. Used by ECD code.
    template<typename S> void injectError( int errorCode, long long timestamp ) {
        static_assert(std::is_base_of<autosar::InTag, S>::value, "Only for IN data elements"); // Make sure we only use this for IN data elements

        auto dev = DataElemValue<S>::ERROR(errorCode, timestamp);
        _inject(dev);
    }

    // Listen to a data-element from the MP application code. Used by the ECD.
    /*
    template<typename S> void sinkSubscribe( INewDataElement* callback ) {
        static_assert(std::is_base_of<autosar::OutTag, S>::value, "Only for OUT data elements"); // make sure this is only used on OUT data elements

        std::unique_ptr<DataElemValueBase> dev(new DataElemValue<S>(DataElemValue<S>::ERROR(-1)));
        subscribe(dev, callback);
    }*/


    // ================================================================
    // Common
    // Unsubscribe on this particular callback (not the whole data element as such!)
    void unsubscribe( const std::string& name, INewDataElement* callback );

    /**
     * @brief resetData Resets all subscriptions and data to empty
     */
    void reset(); // for UT
    int count(); // for UT

private:
    DataElementFramework();

    void _inject(const DataElemValueBase& dataElem); // used to "inject" both IN and OUT data elements on the internal"bus"
    void subscribe(std::unique_ptr<DataElemValueBase>& dev, INewDataElement* callback);
    const DataElemValueBase* get(const std::string& name) const;

    // instance variables
    using MapValue = std::pair<std::unique_ptr<DataElemValueBase>,std::list<INewDataElement*>>;
    std::map<std::string, MapValue> m_dataElementListeners;

    IDataElementCommBus* m_commBus;

    mutable std::recursive_mutex mutex_protectDataElementListeners;
};

#endif // DATAELEMENTFRAMEWORK_H
