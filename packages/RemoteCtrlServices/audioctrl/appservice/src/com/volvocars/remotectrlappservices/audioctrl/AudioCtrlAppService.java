/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlappservices.audioctrl;

import com.volvocars.remotectrlservices.audioctrl.*;
import android.util.Log;
import android.os.IBinder;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.RemoteException;
import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.media.CarAudioManager;
import android.media.IVolumeController;
import android.media.AudioManager;

public class AudioCtrlAppService extends Service {
  private static final String SYSTEM_SERVICE_PACKAGE_NAME = "com.volvocars.remotectrlservices.audioctrl";
  private static final String TAG = "RemoteCtrlApp.AudioCtrl.Service";
  private static final String OUTGOING_INTENT = "com.volvocars.remotectrlservices.audioctrl.AudioCtrlService.ACTION_VOLUME_CTRL";
  private static final String EXPECTED_INTENT = "com.volvocars.remotectrlappservices.audioctrl.AudioCtrlAppService.ACTION_VOLUME_CTRL";
  private AppApiHandler mAppServiceInterfaceImpl = null;
  private IAudioCtrlService mSystemService = null;
  private Car mCar = null;
  private CarAudioManager mCarAudioManager = null;
  private VolumeCallback mVolumeCallback = new VolumeCallback();
  private long mGainFocusRetryWaitTime = 1000;

  @Override
  public void onCreate() {
    Log.v(TAG, "onCreate");
    super.onCreate();
    mCar = Car.createCar(this, mCarServiceConnection);
    mAppServiceInterfaceImpl = new AppApiHandler(this);
  }

  @Override
  public int onStartCommand(Intent intent, int flags, int startId) {
    Log.v(TAG, "onStartCommand");
    Log.v(TAG, "onCreate::Bind to car service");
    if (!mCar.isConnected() || !mCar.isConnecting()) {
      mCar.connect();
    }
    return START_STICKY;
  }

  @Override
  public void onDestroy() {
    Log.v(TAG, "onDestroy");
    super.onDestroy();
  }

  /**
   * Car Service stuffs
   */
  private final ServiceConnection mCarServiceConnection = new ServiceConnection() {

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
      try {
        mCarAudioManager = (CarAudioManager) mCar.getCarManager(Car.AUDIO_SERVICE);
      } catch (Exception ex) {
        Log.e(TAG, "Cannot get CarAudioManager! : " + ex.getMessage());
        return;
      }

      try {
        mCarAudioManager.setVolumeController(mVolumeCallback);
      } catch (CarNotConnectedException ex) {
        Log.e(TAG, "Cannot set VolumeController! : " + ex.getMessage());
      }

      bindToSystemService();
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
      mCarAudioManager = null;
      try {
        mCarAudioManager.setVolumeController(null);
      } catch (CarNotConnectedException e) {
        Log.e(TAG, "Car is not connected! : " + e.getMessage());
      }
    }
  };

  private void bindToSystemService() {
    Intent appServiceIntent = new Intent(OUTGOING_INTENT);
    appServiceIntent.setPackage(SYSTEM_SERVICE_PACKAGE_NAME);
    bindService(appServiceIntent, mSystemServiceConnection, Context.BIND_AUTO_CREATE);
  }

  private ServiceConnection mSystemServiceConnection = new ServiceConnection() {

    public void onServiceConnected(ComponentName name, IBinder service) {
      mSystemService = IAudioCtrlService.Stub.asInterface(service);
    }

    public void onServiceDisconnected(ComponentName name) {
      Log.e(TAG, "Service has unexpectedly disconnected");
      mSystemService = null;
    }
  };

  @Override
  public IBinder onBind(Intent intent) {
    Log.v(TAG, "onBind");
    if (intent.getAction().equals(EXPECTED_INTENT)) {
      // Return app-service interface
      return mAppServiceInterfaceImpl;
    } else {
      Log.d(TAG, "Received a request to bind to unknown action: " + intent.getAction());
      return null;
    }
  }

  private byte scaleVolume(byte audioContext, byte volumeLevel) throws CarNotConnectedException {
    double maxVolume = mCarAudioManager.getStreamMaxVolume(audioContext);
    return (byte) (((double) volumeLevel / maxVolume) * 100);
  }

  /**
   * Calls towards system service.
   */
  public void updateVolume(byte audioContext) {
    Log.v(TAG, "Update volume: audioContext- " + audioContext);
    try {
      byte volumeLevel = (byte) mCarAudioManager.getStreamVolume(audioContext);
      int scaledVolumeLevel = scaleVolume(audioContext, volumeLevel);
      mSystemService.updateVolume(audioContext, (byte) scaledVolumeLevel);
    } catch (RemoteException | CarNotConnectedException e) {
      Log.e(TAG, "UpdateVolume error : " + e.getMessage());
    }
  }

  public void getVolume(int requestIdentifier, byte audioContext) {
    Log.v(TAG, "Get volume: requestIdentifier- " + requestIdentifier + ", audioContext- " + audioContext);

    new Thread(new Runnable() {
      public void run() {
        boolean status = true;
        int volume = -1; //indicates wrong volumelevel?
        try {
          volume = mCarAudioManager.getStreamVolume(audioContext);
          Log.v(TAG, "Current Volume: " + volume);
        } catch (Exception ex) {
          status = false;
          Log.e(TAG, "Car service getVolume error : " + ex.getMessage());
        }

        //return response
        try {
          mSystemService.sendGetVolumeResp(requestIdentifier, status, audioContext, (byte) volume);
        } catch (RemoteException e) {
          Log.e(TAG, "SendGetVolumeResp error : " + e.getMessage());
        }

      }
    }).start();
  }

  public void setVolume(int requestIdentifier, byte audioContext, byte volumeLevel) {
    Log.v(TAG, "Set volume: requestIdentifier- " + requestIdentifier + ", audioContext- " + audioContext
        + ", volumeLevel- " + volumeLevel);

    //Use carAudioManager to set volume here
    new Thread(new Runnable() {
      public void run() {
        boolean status = true;
        try {
          mCarAudioManager.setStreamVolume(audioContext, volumeLevel, 0);
        } catch (Exception e) {
          Log.e(TAG, "Car service setVolume error : " + e.getMessage());
          status = false;
        }

        try {
          //return response
          mSystemService.sendSetVolumeResp(requestIdentifier, status);
        } catch (RemoteException e) {
          Log.e(TAG, "SendSetVolumeResp error +" + e.getMessage());
        }

        // Send notification
        updateVolume(audioContext);
      }
    }).start();

  }

  /**
   * Track change in audio volume
   */
  private class VolumeCallback extends IVolumeController.Stub {
    @Override
    public void displaySafeVolumeWarning(int flags) throws RemoteException {
    }

    @Override
    public void volumeChanged(int streamType, int flags) throws RemoteException {
      Log.i(TAG, "volume change on stream type " + String.valueOf(streamType) + "with flags  " + String.valueOf(flags));
      updateVolume((byte) streamType);
    }

    @Override
    public void masterMuteChanged(int flags) throws RemoteException {
    }

    @Override
    public void setLayoutDirection(int layoutDirection) throws RemoteException {
    }

    @Override
    public void dismiss() throws RemoteException {
    }

    @Override
    public void setA11yMode(int mode) {
    }
  }
}