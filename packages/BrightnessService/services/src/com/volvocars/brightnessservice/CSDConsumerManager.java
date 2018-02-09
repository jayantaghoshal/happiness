/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import java.util.ArrayList;

import android.os.PowerManager;
import android.os.RemoteException;
import android.util.Log;
import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.hardware.automotive.vehicle.V2_0.SubscribeFlags;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.hardware.automotive.vehicle.V2_0.VehicleApPowerState;
import android.hardware.automotive.vehicle.V2_0.VehicleIgnitionState;
import android.hardware.automotive.vehicle.V2_0.VehicleApPowerSetState;
import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;


/**
 * CSDConsumerManager
 */
public class CSDConsumerManager {
    public static final String TAG = "BrightnessService.CSDMa";
    private PowerManager mPowerManager;
    private IVehicle mVehicle;
    private VehicleCallback mInternalCallback;
    private int mSubcribeRetries =0;
    private final int VEHICLE_HAL_SUBSCRIBE_RETRIES = 10;
    private PowerManager.WakeLock wakeLock = null;
    private int CC100 = 0; //LocalConfig value
    private boolean mTimer1Expired = false;
    private boolean mTimer2Expired = false;
    private UberRequest mUberRequest = UberRequest.NotReceived;
    private enum UberRequest{
        NotReceived,
        SetCSDStateInON,
        SetCSDStateInOFF
    }
    public CSDConsumerManager(IVehicle vehicle, PowerManager powerManager) {
        try {
            Log.d(TAG, "BrightnessService.CSDMan start ");
            mVehicle = vehicle;
            mPowerManager = powerManager;
            mInternalCallback = new VehicleCallback();

            //Setup SubscribeOptions
            ArrayList<SubscribeOptions> options = new ArrayList<>();
            SubscribeOptions opts = new SubscribeOptions();
            opts.propId = VehicleProperty.AP_POWER_STATE;
            opts.flags = SubscribeFlags.DEFAULT;
            options.add(opts);
            SubscribeOptions opts2 = new SubscribeOptions();
            opts2.propId = VehicleProperty.IGNITION_STATE;
            opts2.flags = SubscribeFlags.DEFAULT;
            options.add(opts2);

            //subscribe to vehicleproperty events.
            while (mSubcribeRetries < VEHICLE_HAL_SUBSCRIBE_RETRIES) {
                int subcribeResult = mVehicle.subscribe(mInternalCallback, options);
                if (subcribeResult != 0){
                    Log.e(TAG, "subscribe failed: " + subcribeResult + ", mSubcribeRetries " + mSubcribeRetries);
                    mSubcribeRetries++;
                    Thread.sleep(6000);
                }
                else {
                    //Since wo dont get latest values when we subscribe we do get on each property
                    VehiclePropValue requestedprop = new VehiclePropValue();
                    requestedprop.prop = VehicleProperty.AP_POWER_STATE;
                    ValueResult vehicleApPowerStateprop = getVehiclePropValue(requestedprop);
                    int vehicleApPowerState = vehicleApPowerStateprop.propValue.value.int32Values.get(0);
                    onApPowerStateChange(vehicleApPowerState);

                    requestedprop.prop = VehicleProperty.IGNITION_STATE;
                    ValueResult vehicleignitionstateprop2 = getVehiclePropValue(requestedprop);
                    int ignitionState = vehicleignitionstateprop2.propValue.value.int32Values.get(0);
                    onIgnitionChange(ignitionState);

                    Log.v(TAG, "vehicleApPowerState " + vehicleApPowerState);
                    Log.v(TAG, "ignitionstate " + ignitionState);
                    break;
                }
           }
        } catch (RemoteException ex) {
            Log.d(TAG, ex.getMessage());
        }
        catch (InterruptedException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }
    /**
     * Helper function for setting property in Vehicle hal .
     * @param propertyInt is the VehicleProperty id.
     * @param valueToSet is the wanted value for the property,
     * this function only sets the int32value!
     * */
    private void setVehiclePropValue(int propertyInt, int valueToSet) {
        try {
            VehiclePropValue setprop = new VehiclePropValue();
            setprop.prop = propertyInt;
            setprop.value.int32Values.add(valueToSet);
            setprop.areaId = VehicleArea.GLOBAL;
            int setResult = mVehicle.set(setprop);
            Log.v(TAG, "set result " + setResult);
        } catch (RemoteException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }

    /**
     * Helper function for doing a get against Vehicle hal.
     * @param requestedPropValue is VehiclePropValue containing the wanted VehicleProperty
     * */
    private ValueResult getVehiclePropValue(VehiclePropValue requestedPropValue) {
        final ValueResult result = new ValueResult();
        try {
            mVehicle.get(requestedPropValue,
                    (status, propValue) -> {
                        result.status = status;
                        result.propValue = propValue;
                    });
        } catch (RemoteException e) {
            Log.e(TAG, "Failed to get value from vehicle HAL", e);
            result.status = 0;
        }
        return result;
    }
    private static class ValueResult {
        int status;
        VehiclePropValue propValue;
    }
    /**
     * Called on VehicleIgnitionState change
     * @param ignitionState int with ignitionstate
     * */
    private void onIgnitionChange(int ignitionState){
        switch (ignitionState){
            case VehicleIgnitionState.ON:
            case VehicleIgnitionState.START:
                if(wakeLock==null) {
                    wakeLock = mPowerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "VehicleIgnitionOn");
                    wakeLock.acquire();
                }
                break;
            default:
                if(wakeLock!= null) {
                    wakeLock.release();
                    wakeLock = null;
                }
                break;
        }
    }
    /**
     * * Called on VehicleApPowerState change
     * @param powerstate VehicleApPowerState
     * */
    private void onApPowerStateChange(int powerstate){
        switch (powerstate){
            case VehicleApPowerState.ON_DISP_OFF:
                setVehiclePropValue(VehicleProperty.AP_POWER_STATE, VehicleApPowerSetState.DISPLAY_OFF);
                break;
            case VehicleApPowerState.ON_FULL:
                setVehiclePropValue(VehicleProperty.AP_POWER_STATE, VehicleApPowerSetState.DISPLAY_ON);
                break;
            default:
                break;
        }
    }
    /**
     * Callback class used when subscribing to property events in VehicleHal.
     * onPropertyEvent we call onApPowerStateChange or onIgnitionChange depending on type.
     * */
    private class VehicleCallback extends IVehicleCallback.Stub {
        @Override
        public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
            for (VehiclePropValue propValue : propValues) {
                if(propValue.prop == VehicleProperty.IGNITION_STATE){
                    int ignitionState = propValues.get(0).value.int32Values.get(0);
                    onIgnitionChange(ignitionState);
                }
                else if(propValue.prop == VehicleProperty.AP_POWER_STATE){
                    int vehicleApPowerState = propValues.get(0).value.int32Values.get(0);
                    onApPowerStateChange(vehicleApPowerState);
                }
            }
        }
        @Override
        public void onPropertySet(VehiclePropValue propValue) {}//Not used
        @Override
        public void onPropertySetError(int errorCode, int propId, int areaId) {}// Not used
    }
}