/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.app.Service;
import android.content.Intent;
import android.hardware.light.V2_0.ILight;
import android.hardware.light.V2_0.LightState;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import android.os.Process;

/**
 * BrightnessService is a service that controls the brightness of the screen.
*/
public class BrightnessService extends Service {

    public static final String TAG = "BrightnessService";
    private ILight mLight = null;
    private CSDConsumerManager mCSDConsumerManager;
    private IlluminationControl mIlluminationControl;
    private int mLightType = 0;
    private Looper mServiceLooper = null;
    private ServiceHandler mServiceHandler = null;
    private int mIlluminationControlProposal = 0;
    private int mCSDConsumerManagerProposal = 255;
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

            if(msg.arg1 == MessageSender.CSDConsumerManager.ordinal()){
                mCSDConsumerManagerProposal = msg.arg2;
            }
            else if(msg.arg1 ==  MessageSender.IlluminationControl.ordinal()){
                mIlluminationControlProposal = msg.arg2;
            }
            Log.v(TAG,"mIlluminationControlProposal: " + mIlluminationControlProposal);
            Log.v(TAG,"mCSDConsumerManagerProposal: " + mCSDConsumerManagerProposal);
            changeBrightness(Math.min(mCSDConsumerManagerProposal,mIlluminationControlProposal));
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
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(TAG, "onCreate");
        try {
            mLight = ILight.getService();
            mLightType = mLight.getSupportedTypes().get(0);
        }
        catch (RemoteException ex) {
            Log.e(TAG, ex.getMessage());
        }

        // Start up the thread running the service.
        HandlerThread serviceThread = new HandlerThread("serviceThread", Process.THREAD_PRIORITY_BACKGROUND);
        serviceThread.start();
        // Get the HandlerThread's Looper and use it for our Handler
        mServiceLooper = serviceThread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);

        mCSDConsumerManager = new CSDConsumerManager();
        mIlluminationControl = new IlluminationControl(mServiceHandler);

    }
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
    /**
     * Change brightness of the screen, uses lightshal.
     * @param brightnessValue int beetween 0-255
     */
    public void changeBrightness(int brightnessValue){
        try {
            if(brightnessValue < 0 | brightnessValue > 255){
                Log.w(TAG, "brightnessValue not witheen range: " + brightnessValue);
                return;
            }

            Log.v(TAG, "setting brightness to: " + brightnessValue);
            LightState lightState = new LightState();
            int color = brightnessValue & 0x000000ff;
            color = 0xff000000 | (color << 16) | (color << 8) | color;
            lightState.color = color;
            lightState.flashMode = 0;
            lightState.flashOnMs = 0;
            lightState.flashOffMs = 0;
            lightState.brightnessMode = 0;
            mLight.setLight(mLightType, lightState);
        } catch(RemoteException ex){
            Log.d(TAG,ex.getMessage());
        }
    }
}