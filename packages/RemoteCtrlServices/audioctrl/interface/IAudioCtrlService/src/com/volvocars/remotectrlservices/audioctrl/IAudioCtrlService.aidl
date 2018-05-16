package com.volvocars.remotectrlservices.audioctrl;

/**
* An interface to access RemoteAudioCtrl features
*/
interface IAudioCtrlService {

  /**
  * Issue notification about changes in volume level
  * @param level represents the new volume level
  */
  void updateVolume(int volumeLevel);

  /**
  * Report a response to pending "getVolume" request.
  *
  * @param requestIdentifier used to map response to pending request
  * @param requestStatus represents if request was successful or failed.
  *                Other parameters are only valid if status is true.
  * @param volumeLevel current audio volume level
  */
  void sendGetVolumeResp(int requestIdentifier, boolean requestStatus, int volumeLevel);

  /**
  * Report a response to pending "setVolume" request.
  *
  * @param requestIdentifier used to map response to pending request
  * @param requestStatus represents if request was successful or failed
  */
  void sendSetVolumeResp(int requestIdentifier, boolean requestStatus);
}
