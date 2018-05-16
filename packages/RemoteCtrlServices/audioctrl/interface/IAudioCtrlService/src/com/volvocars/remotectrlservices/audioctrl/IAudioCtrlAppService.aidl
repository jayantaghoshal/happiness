package com.volvocars.remotectrlservices.audioctrl;


interface IAudioCtrlAppService {

  /**
  * Get volume of specified audio context
  *
  * @param requestIdentifier identifier used to map response to this request
  */
  void getVolume(int requestIdentifier);

  /**
  * Set volume of specified audio context
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param volumeLevel represents volume level to set
  */
  void setVolume(int requestIdentifier, int volumeLevel);
}
