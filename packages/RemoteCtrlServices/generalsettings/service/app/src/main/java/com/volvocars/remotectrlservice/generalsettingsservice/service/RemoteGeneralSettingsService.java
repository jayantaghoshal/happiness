/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service;

import static vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty.REMOTECTRLHAL_CSD_ON;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.UserHandle;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyResponseService;
;
import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyType;
import vendor.volvocars.hardware.remotectrl.V1_0.VehicleArea;

public class RemoteGeneralSettingsService extends Service {
    private static final String TAG = "RemoteCtrl.GeneralSettings.Service";
    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_GATEWAY_PACKAGE =
            "com.volvocars.remotectrlservices.generalsettingsgateway";
    private static final String GENERAL_SETTINGS_SERVICE_PACKAGE =
            "com.volvocars.remotectrlservice.generalsettingsservice.service";
    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_RESPONSE_BIND_INTENT =
            ".RemoteGeneralSettingsGateway.BIND";
    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_BIND_INTENT =
            ".RemoteGeneralSettingsService.BIND";
    private static final String INTENTFILTER_FOR_BROADCAST_RECEIVER =
            "generalsettingsservice.broadcastaction.checkoverlaystate";
    private Context context = null;
    private DisplayOverlay mDisplayOverlay = null;
    private static final String key = "com.volvocars.generalsettingsservice.displayinformation";
    private int displaysettings = 1; // display is on by default(duh)
    private SharedPreferences mSharedPreferences;
    private SharedPreferences.Editor mSharedPreferencesEditor;
    private boolean mRemoteGeneralSettingsResponseServiceBound = false;
    private IRemoteCtrlPropertyResponseService mRemoteGeneralSettingsResponseService = null;
    private UserSwitchReceiver receiver = null;
    // IMPLEMENTATION OF IRemoteGeneralSettingsService Interface //
    private final IRemoteCtrlPropertyService
            .Stub mRemoteGeneralSettingsService = new IRemoteCtrlPropertyService.Stub() {

        @Override
        public void setProperty(int requestIdentifier,
                RemoteCtrlPropertyValue remoteCtrlPropertyValue) throws RemoteException {
            new AsyncTask<Void, Void, Void>() {
                protected void onPreExecute() {
                    // Pre Code
                }

                protected Void doInBackground(Void... unused) {
                    // Background Code
                    try {
                        Log.v(TAG, "Received request " + requestIdentifier + " Turning display "
                                        + remoteCtrlPropertyValue.getValue());

                        RemoteCtrlPropertyValue remoteCtrlPropertyValueForSetProperty =
                                new RemoteCtrlPropertyValue(remoteCtrlPropertyValue.getPropertyId(),
                                        remoteCtrlPropertyValue.getAreaId(),
                                        RemoteCtrlHalPropertyStatus.AVAILABLE, new Integer[0]);

                        mRemoteGeneralSettingsResponseService.sendSetPropertyResponse(
                                requestIdentifier, remoteCtrlPropertyValueForSetProperty);

                    } catch (Exception ex) {
                        Log.e(TAG, "Exception: " + ex.toString());
                    }
                    return null;
                }

                protected void onPostExecute(Void unused) {
                    try {
                        Log.v(TAG, "remoteCtrlPropertyValue.getValue(): "
                                        + remoteCtrlPropertyValue.getValue());
                        RemoteCtrlPropertyValue mRemoteCtrlPropertyValueForNotification;
                        if (remoteCtrlPropertyValue.getValue().equals(0)) {
                            mDisplayOverlay.enableOverlay();
                            mRemoteCtrlPropertyValueForNotification = new RemoteCtrlPropertyValue(
                                    REMOTECTRLHAL_CSD_ON, VehicleArea.GLOBAL,
                                    RemoteCtrlHalPropertyStatus.AVAILABLE, 0);
                        } else {
                            mDisplayOverlay.disableOverlay();
                            mRemoteCtrlPropertyValueForNotification = new RemoteCtrlPropertyValue(
                                    REMOTECTRLHAL_CSD_ON, VehicleArea.GLOBAL,
                                    RemoteCtrlHalPropertyStatus.AVAILABLE, 1);
                        }
                        mRemoteGeneralSettingsResponseService.notifyPropertyChanged(
                                mRemoteCtrlPropertyValueForNotification);
                    } catch (Exception ex) {
                        Log.e(TAG, "Exception: " + ex);
                    }
                }
            }.execute();
        }

        @Override
        public void getProperty(int requestIdentifier,
                RemoteCtrlPropertyValue remoteCtrlPropertyValue) throws RemoteException {
            displaysettings = mSharedPreferences.getInt(key, MODE_PRIVATE);
            Log.v(TAG, "Displaysettings: " + displaysettings);
            RemoteCtrlPropertyValue mRemoteCtrlPropertyValueForGet = new RemoteCtrlPropertyValue(
                    remoteCtrlPropertyValue.getPropertyId(), remoteCtrlPropertyValue.getAreaId(),
                    RemoteCtrlHalPropertyStatus.AVAILABLE, mDisplayOverlay.isOverlayEnabled());

            mRemoteGeneralSettingsResponseService.sendGetPropertyResponse(
                    requestIdentifier, mRemoteCtrlPropertyValueForGet);
        }
    };

    private final BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            /*restricting unauthorized brightness changes*/
            Log.i(TAG, "IS overlay enabled = " + mDisplayOverlay.isOverlayEnabled()
                            + " Received brightness = "
                            + intent.getExtras().getInt("BrightnessValue"));
            if (mDisplayOverlay.isOverlayEnabled()) {
                Settings.System.putIntForUser(context.getContentResolver(),
                        Settings.System.SCREEN_BRIGHTNESS, 0, UserHandle.USER_CURRENT);
            } else {
                Settings.System.putIntForUser(context.getContentResolver(),
                        Settings.System.SCREEN_BRIGHTNESS,
                        intent.getExtras().getInt("BrightnessValue"), UserHandle.USER_CURRENT);
            }
        }
    };

    private ServiceConnection mRemoteGeneralSettingsResponseServiceConnection =
            new ServiceConnection() {
                @Override
                public void onServiceConnected(ComponentName name, IBinder service) {
                    Log.v(TAG, "IRemoteGeneralSettingsResponseControlService connected.");
                    mRemoteGeneralSettingsResponseService =
                            IRemoteCtrlPropertyResponseService.Stub.asInterface(service);
                }

                @Override
                public void onServiceDisconnected(ComponentName name) {
                    Log.v(TAG, "IRemoteGeneralSettingsResponseControlService disconnected.");
                    mRemoteGeneralSettingsResponseService = null;
                    unbindRemoteGeneralSettingsResponseService();
                }
            };

    @Override
    public void onCreate() {
        try {
            Log.v(TAG, "GeneralSettingsService onCreate");
            context = this;
            LocalBroadcastManager.getInstance(this).registerReceiver(
                    mBroadcastReceiver, new IntentFilter(INTENTFILTER_FOR_BROADCAST_RECEIVER));

            Intent checkDisplayBrightnessServiceIntent =
                    new Intent(this, com.volvocars.remotectrlservice.generalsettingsservice.service
                                             .CheckDisplayBrightnessService.class);

            receiver = new UserSwitchReceiver();
            IntentFilter filter = new IntentFilter();
            filter.addAction(Intent.ACTION_USER_BACKGROUND);
            filter.addAction(Intent.ACTION_USER_FOREGROUND);
            registerReceiver(receiver, filter);
            this.startService(checkDisplayBrightnessServiceIntent);
            mSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
            bindRemoteGeneralSettingsResponseService();
            mDisplayOverlay = new DisplayOverlay(this);

            /*checks shared preferences for previously set states for the overlay
            * */
            if (!mSharedPreferences.contains(key)) {
                Log.i(TAG, "Initial Run Creating SharedPreference");
                mSharedPreferencesEditor = mSharedPreferences.edit();
                mSharedPreferencesEditor.putInt(key, displaysettings);
                mSharedPreferencesEditor.apply();
            } else {
                displaysettings = mSharedPreferences.getInt(key, MODE_PRIVATE);
                if (displaysettings == 1) {
                    Log.i(TAG, "Keeping the display on");
                    mDisplayOverlay.disableOverlay();

                    // keep or enable the display overlay
                } else {
                    Log.i(TAG, "Turning Display off ");
                    mDisplayOverlay.enableOverlay();
                }
            }

        } catch (Exception ex) {
            Log.v(TAG, "Exception thrown: " + ex);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        if (intent.getAction().equals(REMOTE_GENERAL_SETTINGS_SERVICE_BIND_INTENT)) {
            mRemoteGeneralSettingsResponseServiceBound = true;
            return mRemoteGeneralSettingsService;
        } else {
            return null;
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "onStart command");
        return Service.START_STICKY;
    }

    /*writing settings to shared preference to retain memory of current state in case of reboot or
     * crash*/
    private void saveSettings(int value) {
        Log.i(TAG, "Saving settings ");
        mSharedPreferencesEditor = mSharedPreferences.edit();
        mSharedPreferencesEditor.putInt(key, value);
        mSharedPreferencesEditor.apply();
    }

    private void bindRemoteGeneralSettingsResponseService() {
        if (mRemoteGeneralSettingsResponseServiceBound) {
            Log.v(TAG, "Already bound to IRemoteGeneralSettingsResponseService");
            return;
        }
        Log.v(TAG, "Binding IRemoteGeneralSettingsResponseService");
        Intent bindServiceIntent = new Intent(REMOTE_GENERAL_SETTINGS_SERVICE_RESPONSE_BIND_INTENT);
        bindServiceIntent.setPackage(REMOTE_GENERAL_SETTINGS_SERVICE_GATEWAY_PACKAGE);
        if (!bindService(bindServiceIntent, mRemoteGeneralSettingsResponseServiceConnection,
                    Context.BIND_AUTO_CREATE)) {
            Log.e(TAG, "Failed to bind IRemoteGeneralSettingsResponseService");
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        LocalBroadcastManager.getInstance(this).unregisterReceiver(mBroadcastReceiver);
        if (mDisplayOverlay.isOverlayEnabled()) {
            saveSettings(0);
        } else {
            saveSettings(1);
        }
        Log.v(TAG, "onDestroy");
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(TAG, "onUnbind");
        if (intent.getAction().equals(REMOTE_GENERAL_SETTINGS_SERVICE_BIND_INTENT)) {
            Log.v(TAG, "Remote GeneralSettings service unbound");
            mRemoteGeneralSettingsResponseServiceBound = false;
            return true;
        }
        return false;
    }

    private void unbindRemoteGeneralSettingsResponseService() {
        unbindService(mRemoteGeneralSettingsResponseServiceConnection);
        Log.v(TAG, "RemoteGeneralSettingsResponse service unbound");
    }
    private class UserSwitchReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (mDisplayOverlay.isOverlayEnabled()) {
                mDisplayOverlay.disableOverlay();
                restartoverlay();
            }
        }
    }

    private void restartoverlay() {
        Intent checkDisplayBrightnessServiceIntent =
                new Intent(this, com.volvocars.remotectrlservice.generalsettingsservice.service
                                         .CheckDisplayBrightnessService.class);

        this.stopService(checkDisplayBrightnessServiceIntent);
        this.startService(checkDisplayBrightnessServiceIntent);
        LocalBroadcastManager.getInstance(this).unregisterReceiver(mBroadcastReceiver);
        LocalBroadcastManager.getInstance(this).registerReceiver(
                mBroadcastReceiver, new IntentFilter(INTENTFILTER_FOR_BROADCAST_RECEIVER));
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                mDisplayOverlay = new DisplayOverlay(context);
                mDisplayOverlay.enableOverlay();
                Log.i(TAG, "TODO(Arjun) 2000ms delay to be fixed with more robust solution");
            }
        }, 2000);
        // TODO(Arjun) 2000ms delay to be fixed with more robust solution
    }
}
