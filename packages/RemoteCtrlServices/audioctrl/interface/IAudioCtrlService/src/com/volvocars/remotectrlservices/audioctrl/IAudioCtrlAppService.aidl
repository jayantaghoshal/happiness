package com.volvocars.remotectrlservices.audioctrl;


interface IAudioCtrlAppService {

  /**
  * Get volume of specified audio context
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param audioContext represents audio context of interest
  */
  void getVolume(int requestIdentifier, byte audioContext);

  /**
  * Set volume of specified audio context
  *
  * @param requestIdentifier identifier used to map response to this request
  * @param audioContext represents audio context to affect
  * @param volumeLevel represents volume level to set
  */
  void setVolume(int requestIdentifier, byte audioContext, byte volumeLevel);
}