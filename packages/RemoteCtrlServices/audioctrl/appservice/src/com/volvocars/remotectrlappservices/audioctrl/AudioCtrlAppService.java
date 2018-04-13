/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlappservices.audioctrl;

import android.app.Service;
import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.media.CarAudioManager;
import android.car.media.ICarAudio;
import android.car.media.ICarVolumeCallback;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.database.ContentObserver;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.IVolumeController;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrlservices.audioctrl.IAudioCtrlService;

public class AudioCtrlAppService extends Service {
    private static final String SYSTEM_SERVICE_PACKAGE_NAME =
            "com.volvocars.remotectrlservices.audioctrl";
    private static final String TAG = "RemoteCtrlApp.AudioCtrl.Service";
    private static final String OUTGOING_INTENT =
            "com.volvocars.remotectrlservices.audioctrl.AudioCtrlService.ACTION_VOLUME_CTRL";
    private static final String EXPECTED_INTENT =
            "com.volvocars.remotectrlappservices.audioctrl.AudioCtrlAppService.ACTION_VOLUME_CTRL";
    private com.volvocars.remotectrlappservices.audioctrl.AppApiHandler mAppServiceInterfaceImpl =
            null;
    private IAudioCtrlService mSystemService = null;
    private Car mCar = null;
    private CarAudioManager mCarAudioManager = null;
    private CarVolumeCallback mVolumeCallback = new CarVolumeCallback();

    private ServiceConnection mSystemServiceConnection = new ServiceConnection() {

        public void onServiceConnected(ComponentName name, IBinder service) {
            mSystemService = IAudioCtrlService.Stub.asInterface(service);
        }

        public void onServiceDisconnected(ComponentName name) {
            Log.e(TAG, "Service has unexpectedly disconnected");
            mSystemService = null;
        }
    };
    /**
     * Car Service stuffs
     */
    private final ServiceConnection mCarServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            try {
                Log.v(TAG, "mcar connected");
                mCarAudioManager = (CarAudioManager) mCar.getCarManager(Car.AUDIO_SERVICE);
            } catch (Exception ex) {
                Log.e(TAG, "Cannot get CarAudioManager! : " + ex.getMessage());
                return;
            }

            try {
                mCarAudioManager.registerVolumeCallback(mVolumeCallback);
                Log.v(TAG, "Callback registered");

            } catch (CarNotConnectedException ex) {
                Log.e(TAG, "Cannot set VolumeController! : " + ex.getMessage());
            }

            bindToSystemService();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mCarAudioManager = null;
            try {
                mCarAudioManager.unregisterVolumeCallback(mVolumeCallback);
            } catch (CarNotConnectedException e) {
                Log.e(TAG, "Car is not connected! : " + e.getMessage());
            }
        }
    };

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
            Log.v(TAG, "car connected");
        }
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.v(TAG, "onDestroy");
        super.onDestroy();
    }

    private void bindToSystemService() {
        Log.v(TAG, "binding to system service");
        Intent appServiceIntent = new Intent(OUTGOING_INTENT);
        appServiceIntent.setPackage(SYSTEM_SERVICE_PACKAGE_NAME);
        bindService(appServiceIntent, mSystemServiceConnection, Context.BIND_AUTO_CREATE);
    }

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

    /**
     * Calls towards system service.
     */
    public void updateVolume(byte audioContext) {
        try {
            int volumeGroupId = mCarAudioManager.getVolumeGroupIdForUsage(audioContext);
            byte volumeLevel = (byte) mCarAudioManager.getGroupVolume(volumeGroupId);
            Log.v(TAG,
                    "Update volume: audioContext- " + audioContext + " newvolume " + volumeLevel);

            mSystemService.updateVolume(audioContext, volumeLevel);
        } catch (RemoteException | CarNotConnectedException e) {
            Log.e(TAG, "UpdateVolume error : " + e.getMessage());
        }
    }

    public void getVolume(int requestIdentifier, byte audioContext)
            throws CarNotConnectedException {
        Log.v(TAG, "Get volume: requestIdentifier- " + requestIdentifier + ", audioContext- "
                        + audioContext);

        new Thread(new Runnable() {
            public void run() {
                boolean status = true;
                int volume = -1; // indicates wrong volumelevel?

                try {
                    int volumeGroupId = mCarAudioManager.getVolumeGroupIdForUsage(audioContext);

                    volume = mCarAudioManager.getGroupVolume(volumeGroupId);
                    /*    volume = upScaleVolume(
                                (byte)volumeGroupId, (byte) mCarAudioManager.getGroupVolume(
                       volumeGroupId));
                    */
                    Log.v(TAG, "Current Volume: " + volume);
                } catch (Exception ex) {
                    status = false;
                    Log.e(TAG, "Car service getVolume error : " + ex.getMessage());
                }

                // return response
                try {
                    byte bytevolume = (byte) volume;
                    mSystemService.sendGetVolumeResp(
                            requestIdentifier, status, audioContext, bytevolume);
                } catch (RemoteException e) {
                    Log.e(TAG, "SendGetVolumeResp error : " + e.getMessage());
                }
            }
        }).start();
    }

    public void setVolume(int requestIdentifier, byte audioContext, byte volumeLevel)
            throws CarNotConnectedException {
        Log.v(TAG, "Set volume: requestIdentifier- " + requestIdentifier + ", audioContext- "
                        + audioContext + ", volumeLevel- " + volumeLevel);

        // Use carAudioManager to set volume here
        int volumeGroupId = mCarAudioManager.getVolumeGroupIdForUsage(audioContext);
        Log.d(TAG, "Max vol of stream" + String.valueOf(volumeGroupId) + ""
                        + String.valueOf(mCarAudioManager.getGroupMaxVolume(volumeGroupId)));
        new Thread(new Runnable() {
            public void run() {
                boolean status = true;
                try {
                    byte actualVolume = volumeLevel;
                    mCarAudioManager.setGroupVolume(volumeGroupId, actualVolume, 0);
                    Log.d(TAG, "Max vol of stream" + String.valueOf(volumeGroupId) + " is "
                                    + String.valueOf(
                                              mCarAudioManager.getGroupMaxVolume(volumeGroupId)));

                } catch (Exception e) {
                    Log.e(TAG, "Car service setVolume error : " + e.getMessage());
                    status = false;
                }

                try {
                    // return response
                    mSystemService.sendSetVolumeResp(requestIdentifier, status);
                } catch (RemoteException e) {
                    Log.e(TAG, "SendSetVolumeResp error +" + e.getMessage());
                }
                byte bytecontext = (byte) volumeGroupId;
                // Send notification
                updateVolume(bytecontext);
            }
        }).start();
    }

    /**
     * Track change in audio volume
     */
    private final class CarVolumeCallback extends ICarVolumeCallback.Stub {
        @Override
        public void onGroupVolumeChanged(int groupId) throws RemoteException {
            Log.i(TAG, "Volume change on stream type " + String.valueOf(groupId));
            try {
                int[] usage = mCarAudioManager.getUsagesForVolumeGroupId(groupId);
                for (int musage : usage) {
                    if (musage == AudioAttributes.USAGE_MEDIA
                            || musage == AudioAttributes.USAGE_UNKNOWN) {
                        updateVolume((byte) musage);
                    }
                }

            } catch (CarNotConnectedException e) {
                Log.e(TAG, "Car not connected");
            }
        }

        @Override
        public void onMasterMuteChanged() throws RemoteException {}
    }
}
