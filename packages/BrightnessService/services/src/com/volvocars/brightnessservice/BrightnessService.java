/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.UserHandle;
import android.provider.Settings;
import android.util.Log;
import android.os.Process;
import java.util.NoSuchElementException;
import android.hardware.automotive.vehicle.V2_0.IVehicle;

import android.os.HwBinder;


/**
 *
 * BrightnessService is a service that controls the brightness of the screen.
*/
public class BrightnessService extends Service implements HwBinder.DeathRecipient {

    public static final String TAG = "BrightnessService";
    private IVehicle mVehicle = null;
    private CSDConsumerManager mCSDConsumerManager;
    private IlluminationControl mIlluminationControl;
    private CleaningModeOverlay mCleaningModeOverlay;
    private Looper mServiceLooper = null;
    private ServiceHandler mServiceHandler = null;
    private PowerManager mPowerManager;
    private int mIlluminationControlProposal = 255;
    public enum MessageSender{
        CSDConsumerManager,
        IlluminationControl
    }
    public final class ServiceHandler extends Handler {
        public ServiceHandler(Looper looper) {
            super(looper);
        }
        @Override
        public void handleMessage(Message msg) {
            if(msg.arg1 ==  MessageSender.IlluminationControl.ordinal()){
                mIlluminationControlProposal = msg.arg2;
            }
            Log.v(TAG,"mIlluminationControlProposal: " + mIlluminationControlProposal);
            changeBrightness(mIlluminationControlProposal);
        }
    }
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "onStartCommand");
        return START_STICKY;
    }
    @Override
    public void onDestroy() {
        Log.v(TAG, "onDestroy");
        System.exit(0);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(TAG, "onCreate");

        // Start up the thread running the service.
        HandlerThread serviceThread = new HandlerThread("serviceThread", Process.THREAD_PRIORITY_BACKGROUND);
        serviceThread.start();
        // Get the HandlerThread's Looper and use it for our Handler
        mServiceLooper = serviceThread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);
        while (true) {
            try {
                mVehicle=IVehicle.getService();

                if(mVehicle != null){
                    mVehicle.linkToDeath(this, 1010 /* dummy cookie */);
                    break;
                }
                Thread.sleep(1000);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
            catch(RemoteException ex){
                Log.e(TAG,ex.getMessage());
            }
            catch(NoSuchElementException ex){
                Log.e(TAG, "No such element exception, VHAL DOWN");
            }
        }
        mPowerManager = (PowerManager) getSystemService(POWER_SERVICE);
        mCSDConsumerManager = new CSDConsumerManager(mVehicle,mPowerManager);
        mIlluminationControl = new IlluminationControl(mServiceHandler,mVehicle);
        mCleaningModeOverlay = new CleaningModeOverlay(mVehicle,this);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void serviceDied(long cookie) {
        Log.i(TAG, "Lost Connection to VHAL, re-spawning myself");
        stopSelf();
    }

    /**
     * Change brightness of the screen, uses settings&/& lightshal.
     * @param brightnessValue int beetween 0-255
     */
    public void changeBrightness(int brightnessValue){
        if(brightnessValue < 0 || brightnessValue > 255){
            Log.w(TAG, "brightnessValue not within range: " + brightnessValue);
            return;
        }
        try {
            int brightnessMode = Settings.System.getInt(getContentResolver(), Settings.System.SCREEN_BRIGHTNESS_MODE);
            if (brightnessMode == Settings.System.SCREEN_BRIGHTNESS_MODE_AUTOMATIC) {
                Settings.System.putIntForUser(
                    getContentResolver(),
                    Settings.System.SCREEN_BRIGHTNESS_MODE,
                    Settings.System.SCREEN_BRIGHTNESS_MODE_MANUAL,
                    UserHandle.USER_CURRENT);
            }
            Settings.System.putIntForUser(
                getContentResolver(),
                Settings.System.SCREEN_BRIGHTNESS,
                brightnessValue,
                UserHandle.USER_CURRENT);
        }
        catch (Settings.SettingNotFoundException e) {
            Log.e(TAG,e.getMessage());
        }
    }
}