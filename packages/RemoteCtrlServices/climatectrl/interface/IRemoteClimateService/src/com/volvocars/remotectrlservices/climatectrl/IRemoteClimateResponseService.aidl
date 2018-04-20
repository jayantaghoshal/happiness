package com.volvocars.remotectrlservices.climatectrl;

import android.car.hardware.CarPropertyValue;

/**
* An interface to access RemoteClimateCtrl features
*/
interface IRemoteClimateResponseService {

  /**
  * Report a response to pending set request
  *
  * @param requestIdentifier identifier used to map response to request
  * @param propValue return value of set request, including status
  */
  void sendSetPropertyResponse(int requestIdentifier, in CarPropertyValue propValue);

  /**
  * Report a response to a pending get request
  *
  * @param requestIdentifier identifier used to map response to request
  * @param requestedPropValue return value of get request, including status
  */
  void sendGetPropertyResponse(int requestIdentifier, in CarPropertyValue requestedPropValue);

  /**
  * Issue notification about change of car hvac value
  *
  * @param propValue notification value, including status
  */
  void notifyPropertyChanged(in CarPropertyValue propValue);
}

