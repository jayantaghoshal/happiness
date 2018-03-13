/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnesstest;
import android.app.Activity;
import android.database.ContentObserver;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.util.Log;

public class IlluminationControl extends Activity {
    private String TAG = "IlluminationControlTestApp";

    public interface OnBrightnessChange{
        void onChange();
    }
    private OnBrightnessChange OnBrightnessChangeImpl = null;
    private ContentObserver mBrightnessObserver = new ContentObserver(new Handler()) {
        @Override
        public void onChange(boolean selfChange) {
            Log.d(TAG," " + getBrightness());
            OnBrightnessChangeImpl.onChange();
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
    @Override
    protected void onStart() {
        super.onStart();
    }
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG,"InBackground");
    }

    public int getBrightness(){
        int brightness = -1;
        try {
             brightness = Settings.System.getInt(getContentResolver(), Settings.System.SCREEN_BRIGHTNESS);
        } catch (Settings.SettingNotFoundException e) {
            e.printStackTrace();
        }
        finally {
            return brightness;
        }
    }
    public void registerChangeCallback(OnBrightnessChange onBrightnessChange) throws InterruptedException {
        getContentResolver().registerContentObserver(
                Settings.System.getUriFor(Settings.System.SCREEN_BRIGHTNESS), true,
                mBrightnessObserver);
        OnBrightnessChangeImpl = onBrightnessChange;
    }
    public void unRegisterChangeCallback() throws InterruptedException {
        getContentResolver().unregisterContentObserver(mBrightnessObserver);
        OnBrightnessChangeImpl = null;
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG,"InForeground");
    }

}
