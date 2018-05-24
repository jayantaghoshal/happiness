/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.os.UserHandle;
import android.provider.Settings;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
/*Overlay over display to mimic screen off state. does not restrict background processes */
public class DisplayOverlay {
    private String TAG = "RemoteCtrl.GeneralSettings.Service.ChangeDisplayState";
    public WindowManager.LayoutParams mLayoutParams;
    public WindowManager mWindowManager;
    int TYPE_DISPLAY_OVERLAY = WindowManager.LayoutParams.FIRST_SYSTEM_WINDOW + 26;
    private Context mContext;
    private LinearLayout mOverlayView;

    public DisplayOverlay(Context context) {
        Log.i(TAG, "Initialized DisplayState Changer");
        mContext = context;
        mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
    }

    /**
     * Change brightness of the screen, uses settings
     *
     * @param brightnessValue int beetween 0-255
     */
    public void changeBrightness(int brightnessValue) {
        if (brightnessValue < 0 || brightnessValue > 255) {
            Log.w(TAG, "brightnessValue not within range: " + brightnessValue);
            return;
        }
        try {
            int brightnessMode = Settings.System.getInt(
                    mContext.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS_MODE);
            if (brightnessMode == Settings.System.SCREEN_BRIGHTNESS_MODE_AUTOMATIC) {
                Settings.System.putIntForUser(mContext.getContentResolver(),
                        Settings.System.SCREEN_BRIGHTNESS_MODE,
                        Settings.System.SCREEN_BRIGHTNESS_MODE_MANUAL, UserHandle.USER_CURRENT);
            }
            Settings.System.putIntForUser(mContext.getContentResolver(),
                    Settings.System.SCREEN_BRIGHTNESS, brightnessValue, UserHandle.USER_CURRENT);
        } catch (Settings.SettingNotFoundException e) {
            Log.e(TAG, e.getMessage());
        }
        return;
    }

    /*Setting the parameters for the overlay which mimics a screen off state*/

    private void setParamsForOverlay() {
        mOverlayView = new LinearLayout(mContext);
        mOverlayView.setBackgroundColor(Color.BLACK);
        mOverlayView.setVerticalGravity(Gravity.CENTER_VERTICAL);
        mOverlayView.setOnKeyListener((v, keyCode, event) -> {
            Log.v(TAG, "onKey discarding");
            return false;
        });
        mLayoutParams = new WindowManager.LayoutParams(WindowManager.LayoutParams.MATCH_PARENT,
                            WindowManager.LayoutParams.MATCH_PARENT, TYPE_DISPLAY_OVERLAY,
                     WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
                            PixelFormat.TRANSLUCENT);
        Point size = new Point();
        mWindowManager.getDefaultDisplay().getRealSize(size);
        mLayoutParams.gravity = Gravity.LEFT | Gravity.TOP;
        mLayoutParams.x = 0;
        mLayoutParams.y = -getStatusBarHeight(); // move overlay so its over the statusbar
        mLayoutParams.width = size.x;
        mLayoutParams.height = size.y;
    }

    /*Checks if the display is off*/

    public boolean isOverlayEnabled() {
        if (mOverlayView == null) {
            return false;
        } else {
            return true;
        }
    }

    /*Enables the displayOverlay*/
    public void enableOverlay() {
        changeBrightness(0);
        Log.i(TAG, "enabling the overlay");
        if (mOverlayView == null) {
            setParamsForOverlay();
            mOverlayView.setVisibility(View.VISIBLE);
            mOverlayView.setAlpha(0.90f);
            mWindowManager.addView(mOverlayView, mLayoutParams);
            mOverlayView.animate().alpha(1f);
            Log.v(TAG, "overlayView add");
        }
        return;
    }

    @SuppressLint("NewApi")
    public void disableOverlay() {
        changeBrightness(255);
        Log.i(TAG, "Disabling the overlay");
        if (mOverlayView != null) {
            mOverlayView.animate().alpha(0.0f).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                    mWindowManager.removeView(mOverlayView);
                    mOverlayView = null;
                    mLayoutParams = null;
                }
            });
            Log.v(TAG, "overlayView removed");
        }
        return;
    }

    private int getStatusBarHeight() {
        int result = 0;
        int resourceId =
                mContext.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = mContext.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }
}