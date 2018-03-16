/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.util.Log;
import android.os.RemoteException;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;
import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.hardware.automotive.vehicle.V2_0.SubscribeFlags;
import android.animation.AnimatorListenerAdapter;
import android.animation.Animator;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import java.util.ArrayList;
import android.content.Context;
import android.widget.LinearLayout;
import android.widget.TextView;

public class CleaningModeOverlay {

    private Context mContext;
    private LinearLayout mOverlayView;
    private WindowManager.LayoutParams mLayoutParams;
    private IVehicle mVehicle;
    private WindowManager mWindowManager;
    private String TAG = "Brightness.CleaningMode";
    private final int VEHICLE_HAL_SUBSCRIBE_RETRIES = 10;
    private int mSubcribeRetries = 0;
    private VehicleCallback mInternalCallback = null;
    // Defined here since this flag is hidden:
    int TYPE_DISPLAY_OVERLAY = WindowManager.LayoutParams.FIRST_SYSTEM_WINDOW + 26;
    /**
     *  Handler used to call the UI thread from VehicleCallback background thread.
     **/
    private Handler mHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message message) {
            onHomeButtonStateChange(message.what);
        }
    };
     /**
     * Callback class used when subscribing to property events in VehicleHal.
     * onPropertyEvent we call change the .
     **/
    public class VehicleCallback extends IVehicleCallback.Stub {
        @Override
        public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
            for (VehiclePropValue propValue : propValues) {
                int val = propValue.value.int32Values.get(0);
                Log.v(TAG,"Longpress: " + val);
                Message message = mHandler.obtainMessage(val);
                message.sendToTarget();
            }
        }
        @Override
        public void onPropertySet(VehiclePropValue propValue) {}//Not used
        @Override
        public void onPropertySetError(int errorCode, int propId, int areaId) {}// Not used
    }

    public CleaningModeOverlay(IVehicle vehicle, Context context) {
        mContext = context;
        mVehicle = vehicle;
        mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        mInternalCallback = new VehicleCallback();
        //Setup SubscribeOptions
        ArrayList<SubscribeOptions> options = new ArrayList<>();
        SubscribeOptions opts = new SubscribeOptions();
        opts.propId = VehicleProperty.HOME_KEY_LONGPRESS;
        opts.flags = SubscribeFlags.DEFAULT;
        options.add(opts);

        //subscribe to vehicleproperty events.
        try{
            while (mSubcribeRetries < VEHICLE_HAL_SUBSCRIBE_RETRIES) {
                int subcribeResult = mVehicle.subscribe(mInternalCallback, options);
                if (subcribeResult != 0) { //!=OK
                    Log.e(TAG, "subscribe failed: " + subcribeResult + ", mSubcribeRetries " + mSubcribeRetries);
                    mSubcribeRetries++;
                    Thread.sleep(6000);
                } else {
                    Log.v(TAG, "subscribe success: ");
                    VehiclePropValue requestedprop = new VehiclePropValue();
                    requestedprop.prop = VehicleProperty.HOME_KEY_LONGPRESS;
                    VehicleHalUtils.ValueResult homekeystateprop = VehicleHalUtils.GetVehiclePropValue(mVehicle, requestedprop);
                    if(homekeystateprop.status == 0){
                        int homekeystate = homekeystateprop.propValue.value.int32Values.get(0);
                        Message message = mHandler.obtainMessage(homekeystate);
                        message.sendToTarget();
                    }
                    break;
                }
            }
        }
        catch (RemoteException ex) {
            Log.d(TAG, ex.getMessage());
        }
        catch (InterruptedException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }
    private void setParamsForOverlay(){

        mOverlayView = new LinearLayout(mContext);
        mOverlayView.setBackgroundColor(Color.BLACK);
        mOverlayView.setVerticalGravity(Gravity.CENTER_VERTICAL);
        mOverlayView.setOnKeyListener((v, keyCode, event) -> {
            Log.v(TAG,"onKey discarding");
            return false;
        });
        TextView mtextView = new TextView(mContext);
        mtextView.setText("In screen cleaning mode hold home button to deactivate");
        mtextView.setTextColor(Color.WHITE);
        mtextView.setTextSize(40);
        mtextView.setPadding(30,30,30,30);
        mtextView.setGravity(Gravity.CENTER);
        mOverlayView.addView(mtextView);
        mLayoutParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.MATCH_PARENT,
            TYPE_DISPLAY_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            PixelFormat.TRANSLUCENT
        );
        Point size = new Point();
        mWindowManager.getDefaultDisplay().getRealSize(size);
        mLayoutParams.gravity = Gravity.LEFT | Gravity.TOP;
        mLayoutParams.x = 0;
        mLayoutParams.y = -getStatusBarHeight(); //move overlay so its over the statusbar
        mLayoutParams.width = size.x;
        mLayoutParams.height = size.y;
    }

    @SuppressLint("NewApi")
    private void onHomeButtonStateChange(int cleaningMode){
        if(cleaningMode == 0 && mOverlayView != null){
            mOverlayView.animate()
                .alpha(0.0f)
                .setListener(new AnimatorListenerAdapter() {
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
        else if(cleaningMode == 1 && mOverlayView == null){
            setParamsForOverlay();
            mOverlayView.setVisibility(View.VISIBLE);
            mOverlayView.setAlpha(0.90f);
            mWindowManager.addView(mOverlayView, mLayoutParams);
            mOverlayView.animate().alpha(1f);
            Log.v(TAG, "overlayView add");
        }
    }
    private int getStatusBarHeight() {
        int result = 0;
        int resourceId = mContext.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = mContext.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }
}
