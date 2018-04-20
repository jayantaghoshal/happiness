package com.volvocars.remotectrlservices.climatectrl;

import android.car.hardware.CarPropertyValue;

/**
* An interface to access RemoteClimateCtrl features
*/
interface IRemoteClimateService {
  /**
  * Request a set on the hvac property
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param propValue value of set request
  */
  void setProperty(int requestIdentifier, in CarPropertyValue propValue);

  /**
  * Request a get on the hvac property
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param requestedPropValue value of set request
  */
  void getProperty(int requestIdentifier, in CarPropertyValue requestedPropValue);
}

