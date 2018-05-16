/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.audioctrl;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.VehicleAreaZone;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;
import java.util.Timer;
import java.util.TimerTask;
import java.util.NoSuchElementException;
import android.util.Log;
import android.os.HwBinder;
import android.os.RemoteException;

public class HalHandler {
    private static final String TAG = "RemoteCtrl.AudioCtrl.HalHandler";
    private static final int COOKIE = 1090;
    private IRemoteCtrlPropertyResponse mNativeHalService = null;
    private SystemHalServiceHandler mSystemHalService = null;
    private IAudioCtrlAppService mAppService = null;
    private DeathRecipient mServiceDeath = new DeathRecipient(this);
    private long mConnRetryWaitTime = 1000;
    public boolean mClearToRegisterToDaemon = false;

    public void init() {
        retryConnectToHALService();
    }

    public void setAppService(IAudioCtrlAppService appService) {
        this.mAppService = appService;
    }

    private void setupHALServices() throws RemoteException, NoSuchElementException {
        Log.v(TAG, "Register mSystemHalService as a HAL Service");

        if (mSystemHalService == null) {
            mSystemHalService = new SystemHalServiceHandler();
        }

        Log.v(TAG, "Get NativeRemoteAudioCtrl HAL Service.");
        mNativeHalService = IRemoteCtrlPropertyResponse.getService("RemoteCtrl_AudioCtrl");

        mNativeHalService.linkToDeath(mServiceDeath, COOKIE /* cookie */);

        if (mClearToRegisterToDaemon) {
            registerToNativeService();
        }
    }

    private void retryConnectToHALService() {
        try {
            setupHALServices();
        } catch (NoSuchElementException | RemoteException ex) {
            Log.e(TAG, "Cannot setup HAL services... : " + ex.getMessage());
            new Timer().schedule(new TimerTask() {
                @Override
                public void run() {
                    Log.v(TAG, "Retrying to setup HAL services ...");
                    retryConnectToHALService();
                }
            }, mConnRetryWaitTime);
        }
    }

    public void registerToNativeService() {
        try {
            if (mNativeHalService != null) {
                Log.v(TAG, "Register local service as handler to remove native service");
                mNativeHalService.registerRemoteCtrlPropertyHandler(mSystemHalService);
            } else {
                Log.e(TAG, "Cannot register because HAL service is not available");
                new Timer().schedule(new TimerTask() {
                    @Override
                    public void run() {
                        Log.v(TAG, "Retrying to register to HAL service ...");
                        registerToNativeService();
                    }
                }, mConnRetryWaitTime);
            }
        } catch (RemoteException ex) {
            Log.e(TAG, "Cannot register handler. RemoteExpection: " + ex.getMessage());
        }
    }

    public void unregisterFromNativeService() {
        try {
            Log.v(TAG, "Unregister local service from native service");
            mNativeHalService.unregisterRemoteCtrlPropertyHandler(mSystemHalService);
        } catch (RemoteException ex) {
            Log.e(TAG, "Cannot unregister handler. RemoteExpection: " + ex.getMessage());
        }
    }

    public void nativeRemoteAudioCtrlServiceDied() {
        mNativeHalService = null;
        retryConnectToHALService();
    }

    public void updateVolume(int volumeLevel) {
        try {
            RemoteCtrlHalPropertyValue propValue = new RemoteCtrlHalPropertyValue();
            propValue.prop = RemoteCtrlHalProperty.REMOTECTRLHAL_MEDIA_VOLUME;
            propValue.areaId = VehicleAreaZone.ROW_1_CENTER;
            propValue.status = RemoteCtrlHalPropertyStatus.AVAILABLE;
            propValue.value.int32Values.add(volumeLevel);
            mNativeHalService.notifyPropertyChanged(propValue);
        } catch (RemoteException ex) {
            Log.e(TAG,
                    "Error calling updateVolume function on native HAL interface: "
                            + ex.getMessage());
        }
    }

    public void sendGetVolumeResp(int requestIdentifier, boolean requestStatus, int volumeLevel) {
        try {
            RemoteCtrlHalPropertyValue propValue = new RemoteCtrlHalPropertyValue();
            propValue.prop = RemoteCtrlHalProperty.REMOTECTRLHAL_MEDIA_VOLUME;
            propValue.areaId = VehicleAreaZone.ROW_1_CENTER;
            propValue.status = (requestStatus) ? RemoteCtrlHalPropertyStatus.AVAILABLE
                                               : RemoteCtrlHalPropertyStatus.SYSTEM_ERROR;
            propValue.value.int32Values.add(volumeLevel);
            mNativeHalService.sendGetPropertyResp(requestIdentifier, propValue);
        } catch (RemoteException ex) {
            Log.e(TAG,
                    "Error calling sendGetVolumeResp function on native HAL interface: "
                            + ex.getMessage());
        }
    }

    public void sendSetVolumeResp(int requestIdentifier, boolean requestStatus) {
        try {
            RemoteCtrlHalPropertyValue propValue = new RemoteCtrlHalPropertyValue();
            propValue.prop = RemoteCtrlHalProperty.REMOTECTRLHAL_MEDIA_VOLUME;
            propValue.areaId = VehicleAreaZone.ROW_1_CENTER;
            propValue.status = (requestStatus) ? RemoteCtrlHalPropertyStatus.AVAILABLE
                                               : RemoteCtrlHalPropertyStatus.SYSTEM_ERROR;
            mNativeHalService.sendSetPropertyResp(requestIdentifier, propValue);
        } catch (RemoteException ex) {
            Log.e(TAG,
                    "Error calling sendSetVolumeResp function over native HAL interface: "
                            + ex.getMessage());
        }
    }

    final class DeathRecipient implements HwBinder.DeathRecipient {
        HalHandler remoteClient;

        DeathRecipient(HalHandler client) {
            this.remoteClient = client;
        }

        @Override
        public void serviceDied(long cookie) {
            if (cookie != COOKIE) {
                return;
            }
            Log.e(TAG, "NativeRemoteAudioCtrl HAL service died");
            remoteClient.nativeRemoteAudioCtrlServiceDied();
        }
    }

    public class SystemHalServiceHandler extends IRemoteCtrlProperty.Stub {
        @Override
        public void getProperty(int requestIdentifier, RemoteCtrlHalPropertyValue propValue) {
            Log.v(TAG, "getVolume request received from HAL daemon");
            synchronized (this) {
                try {
                    mAppService.getVolume(requestIdentifier);
                } catch (RemoteException ex) {
                    Log.e(TAG,
                            "Error forwarding GetVolume request to appService : "
                                    + ex.getMessage());
                }
            }
        }

        @Override
        public void setProperty(int requestIdentifier, RemoteCtrlHalPropertyValue propValue) {
            Log.v(TAG, "setVolume request received from HAL daemon");
            synchronized (this) {
                try {
                    RemoteCtrlHalPropertyValue.RawValue value = propValue.value;
                    mAppService.setVolume(requestIdentifier, value.int32Values.get(0));
                } catch (RemoteException ex) {
                    Log.e(TAG,
                            "Error forwarding SetVolume request to appService : "
                                    + ex.getMessage());
                }
            }
        }
    }
}
