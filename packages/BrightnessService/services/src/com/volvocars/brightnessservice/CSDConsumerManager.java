/*
 * Copyright 2017-2018 Volvo Car Corporation
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
import vendor.volvocars.hardware.iplm.V1_0.IIplm;
import vendor.volvocars.hardware.iplm.V1_0.IIplmCallback;
import vendor.volvocars.hardware.iplm.V1_0.ResourceGroup;
import vendor.volvocars.hardware.iplm.V1_0.ResourceGroupPrio;

/**
 *
 * CSDConsumerManager
 */
public class CSDConsumerManager {
    public static final String TAG = "BrightnessService.CSDMa";
    private PowerManager mPowerManager;
    private IVehicle mVehicle;
    private IIplmCallback mIIplmCallback;
    private IIplm mIIplm;
    private VehicleCallback mInternalCallback;
    private IplmCallback miplmCallback;
    private int mSubcribeRetries =0;
    private final int VEHICLE_HAL_SUBSCRIBE_RETRIES = 10;
    private PowerManager.WakeLock wakeLockVehicleIgnition = null;
    private PowerManager.WakeLock wakeLockIplm = null;
    public CSDConsumerManager(IVehicle vehicle, PowerManager powerManager) {
        try {
            Log.d(TAG, "BrightnessService.CSDMan start ");
            mVehicle = vehicle;
            mPowerManager = powerManager;
            mInternalCallback = new VehicleCallback();
            miplmCallback = new IplmCallback();

            mIIplm = IIplm.getService();
            mIIplm.registerService("CSDConsumerManager",miplmCallback);
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
                    VehicleHalUtils.ValueResult vehicleApPowerStateprop = VehicleHalUtils.GetVehiclePropValue(mVehicle,requestedprop);
                    int vehicleApPowerState = vehicleApPowerStateprop.propValue.value.int32Values.get(0);
                    onApPowerStateChange(vehicleApPowerState);

                    requestedprop.prop = VehicleProperty.IGNITION_STATE;
                    VehicleHalUtils.ValueResult vehicleignitionstateprop2 = VehicleHalUtils.GetVehiclePropValue(mVehicle,requestedprop);
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
     * Called on VehicleIgnitionState change
     * @param ignitionState int with ignitionstate
     * */
    private void onIgnitionChange(int ignitionState){
        switch (ignitionState){
            case VehicleIgnitionState.ON:
            case VehicleIgnitionState.START:
                if(wakeLockVehicleIgnition ==null) {
                    wakeLockVehicleIgnition = mPowerManager.newWakeLock(
                        PowerManager.FULL_WAKE_LOCK|PowerManager.ON_AFTER_RELEASE,
                        "VehicleIgnitionOn");
                    wakeLockVehicleIgnition.acquire();
                }
                break;
            default:
                if(wakeLockVehicleIgnition != null) {
                    wakeLockVehicleIgnition.release();
                    wakeLockVehicleIgnition = null;
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
                VehicleHalUtils.SetVehiclePropValue(mVehicle,VehicleProperty.AP_POWER_STATE, VehicleApPowerSetState.DISPLAY_OFF);
                break;
            case VehicleApPowerState.ON_FULL:
                VehicleHalUtils.SetVehiclePropValue(mVehicle,VehicleProperty.AP_POWER_STATE, VehicleApPowerSetState.DISPLAY_ON);
                break;
            default:
                break;
        }
    }
    /**
     * Callback class used when subscribing to property events in VehicleHal.
     * onPropertyEvent we call onApPowerStateChange or onIgnitionChange depending on type.
     * */
    public class VehicleCallback extends IVehicleCallback.Stub {
        @Override
        public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
            for (VehiclePropValue propValue : propValues) {
                if(propValue.prop == VehicleProperty.IGNITION_STATE){
                    int ignitionState = propValue.value.int32Values.get(0);
                    onIgnitionChange(ignitionState);
                }
                else if(propValue.prop == VehicleProperty.AP_POWER_STATE){
                    int vehicleApPowerState = propValue.value.int32Values.get(0);
                    onApPowerStateChange(vehicleApPowerState);
                }
            }
        }
        @Override
        public void onPropertySet(VehiclePropValue propValue) {}//Not used
        @Override
        public void onPropertySetError(int errorCode, int propId, int areaId) {}// Not used
    }
    /**
     * Callback class for iplm.
     * onResourceGroupStatus is used determine if an TCAM Request PRIO_HIGH
     * then we should acquire wakelock to keep the screen lit.
     * */
    private class IplmCallback extends IIplmCallback.Stub{
        @Override
        public void onResourceGroupStatus(byte resourceGroup, byte resourceGroupStatus,byte resourceGroupPrio){
            if(resourceGroup == ResourceGroup.ResourceGroup3 && resourceGroupPrio == ResourceGroupPrio.High && wakeLockIplm == null){
                Log.v(TAG,"acquire wakeLockIplm:");
                Log.d(TAG,"resourceGroup: :" + resourceGroup);
                Log.d(TAG,"resourceGroupStatus: " + resourceGroupStatus);
                Log.d(TAG,"resourceGroupPrio: " + resourceGroupPrio);
                wakeLockIplm = mPowerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "IPLM_RG3_PrioHigh");
                wakeLockIplm.acquire();
            }
            else if(resourceGroup == ResourceGroup.ResourceGroup3  && resourceGroupPrio == ResourceGroupPrio.Normal && wakeLockIplm != null){ // PRIO_HIGH not set anymore.
                Log.v(TAG,"Release wakelock:");
                Log.d(TAG,"resourceGroup: :" + resourceGroup);
                Log.d(TAG,"resourceGroupStatus: " + resourceGroupStatus);
                Log.d(TAG,"resourceGroupPrio: " + resourceGroupPrio);
                wakeLockIplm.release();
                wakeLockIplm = null;
            }
        }
        @Override
        public void onNodeStatus(byte ecuType, boolean ecuStatus){} // not used
    }
}