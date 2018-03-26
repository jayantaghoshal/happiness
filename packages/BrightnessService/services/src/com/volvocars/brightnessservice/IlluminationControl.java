/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.hardware.automotive.vehicle.V2_0.SubscribeFlags;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import java.util.ArrayList;
import java.lang.Thread;
/**
 * IlluminationControl listens to Vehicle hal updates
*/
public class IlluminationControl {
    public static final String TAG = "BrightnessService.IC";

    private IVehicle mVehicle = null;
    private VehicleCallback  mInternalCallback;
    private BrightnessService.ServiceHandler mServiceHandler;
    private int mLatestManualIllumination=101;
    private int mLatestNight=0;//It is day by default;
    private final float PERCENTAGE_OF_MAX_VALUE = 406.0f/1023.0f; // Percentage of max CSD brightness.
    private final float SCALED_MAX_VALUE = PERCENTAGE_OF_MAX_VALUE * 255; // whats the max manual illumination in android scale?
    private final int VEHICLE_HAL_SUBSCRIBE_RETRIES = 10;
    private int mSubcribeRetries = 0;

    public IlluminationControl(BrightnessService.ServiceHandler serviceHandler, IVehicle vehicle) {
        try {
            Log.v(TAG, "IlluminationControl start ");
            mServiceHandler = serviceHandler;
            mVehicle = vehicle;
            mInternalCallback = new VehicleCallback();

            //Setup SubscribeOptions
            ArrayList<SubscribeOptions> options = new ArrayList<>();
            SubscribeOptions opts = new SubscribeOptions();
            opts.propId = VehicleProperty.MANUAL_ILLUMINATION;
            opts.flags = SubscribeFlags.DEFAULT;
            options.add(opts);
            SubscribeOptions opts2 = new SubscribeOptions();
            opts2.propId = VehicleProperty.NIGHT_MODE;
            opts2.flags = SubscribeFlags.DEFAULT;
            options.add(opts2);

            //subscribe to vehicleproperty events.
            while (mSubcribeRetries < VEHICLE_HAL_SUBSCRIBE_RETRIES) {
                int subcribeResult = mVehicle.subscribe(mInternalCallback, options);
                if (subcribeResult != 0) { //!=OK
                    Log.e(TAG, "subscribe failed: " + subcribeResult + ", mSubcribeRetries " + mSubcribeRetries);
                    mSubcribeRetries++;
                    Thread.sleep(6000);
                }
                else{
                    //Since wo dont get latest values when we subscribe we do get on each property
                    VehiclePropValue requestedprop = new VehiclePropValue();
                    requestedprop.prop = VehicleProperty.NIGHT_MODE;
                    VehicleHalUtils.ValueResult valueResultNightMode = VehicleHalUtils.GetVehiclePropValue(mVehicle,requestedprop);
                    requestedprop.prop = VehicleProperty.MANUAL_ILLUMINATION;
                    VehicleHalUtils.ValueResult propValueManual = VehicleHalUtils.GetVehiclePropValue(mVehicle,requestedprop);
                    Log.v(TAG, "propValueManual " + propValueManual.propValue.value.toString());
                    Log.v(TAG, "valueResultNightMode " + valueResultNightMode.propValue.value.toString());

                    ArrayList<VehiclePropValue> vehicleProps = new ArrayList<>();
                    if(propValueManual.status == 0)//==OK
                        vehicleProps.add(propValueManual.propValue);
                    if(valueResultNightMode.status == 0)
                        vehicleProps.add(valueResultNightMode.propValue);

                    if(vehicleProps.size() > 0) {
                        Message msg = new Message();
                        msg.arg1 = BrightnessService.MessageSender.IlluminationControl.ordinal();
                        msg.arg2 = getMaxBrightnessPropValue(vehicleProps);
                        mServiceHandler.sendMessage(msg);
                    }
                    break;
                }
                Log.e(TAG, "subscribe failed permanently: ");
            }
        }
        catch (RemoteException ex) {
            Log.e(TAG, ex.getMessage());
        }
        catch (InterruptedException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }
    /**
     * Gets that maximum brightness value from array list och VehiclePropValues
     * @param propValues array list och VehiclePropValues
     */
    private int getMaxBrightnessPropValue(ArrayList<VehiclePropValue> propValues){
        int max = 0; //255;
        for(VehiclePropValue prop: propValues) {
            if (prop.prop == VehicleProperty.NIGHT_MODE){
                if (prop.value.int32Values.get(0) == 1) {
                    mLatestNight = 1;
                }
                else{
                    mLatestNight = 0;
                }
                Log.v(TAG, "NIGHT_MODE " + prop.value.int32Values.get(0));
            }
            else if (prop.prop == VehicleProperty.MANUAL_ILLUMINATION) {
                //Note.
                //The csd max Value is 1023
                //In Android, sceeen brightness is between 0 - 255
                //VehicleProperty.MANUAL_ILLUMINATION is defined as a value between 0-100
                //which is the percentage value of full manual illumination which is max 406.
                //We need to convert CSD brightness to Android scale. This is the variable SCALED_MAX_VALUE
                float ManualPercentage = (float)prop.value.int32Values.get(0) /100.0f; //Int to float conversion
                float BrightnessValue = SCALED_MAX_VALUE * ManualPercentage; //new proposed brightness
                mLatestManualIllumination = Math.round(BrightnessValue);
                Log.v(TAG, "MANUAL_ILLUMINATION ManualIntPercentage " + ManualPercentage);
                Log.v(TAG, "MANUAL_ILLUMINATION BrightnessValue " +BrightnessValue);
            }
        }
        if(mLatestNight == 0){// Its day so max brightness
            max = 255 ;
        }
        else{
            max = mLatestManualIllumination; //Its night so user decide
        }
        Log.v(TAG, "max is " + max);
        return max;
    }
    /**
     * Callback function used when subscribing to property events in VehicleHal.
     * onPropertyEvent we send the values to BrightnessService.
     * */
    private class VehicleCallback extends IVehicleCallback.Stub {
        @Override
        public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
            int max = getMaxBrightnessPropValue(propValues);
            Message msg = new Message();
            msg.arg1 = BrightnessService.MessageSender.IlluminationControl.ordinal();
            msg.arg2 = max;
            mServiceHandler.sendMessage(msg);
        }
        @Override
        public void onPropertySet(VehiclePropValue propValue) {// Not Used
        }
        @Override
        public void onPropertySetError(int errorCode, int propId, int areaId) {// Not Used
        }
    }
}
