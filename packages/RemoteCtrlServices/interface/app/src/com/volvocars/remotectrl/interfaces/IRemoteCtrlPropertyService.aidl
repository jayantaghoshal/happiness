package com.volvocars.remotectrl.interfaces;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

/**
* An interface to access RemoteCtrlPropertyService features
*/
interface IRemoteCtrlPropertyService {
  /**
  * Request a set on the hvac property
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param propValue value of set request
  */
  void setProperty(int requestIdentifier, in RemoteCtrlPropertyValue propValue);

  /**
  * Request a get on the hvac property
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param requestedPropValue value of set request
  */
  void getProperty(int requestIdentifier, in RemoteCtrlPropertyValue requestedPropValue);
}
