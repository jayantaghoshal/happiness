/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.VehicleApPowerSetState;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.hardware.automotive.vehicle.V2_0.VehicleIgnitionState;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.VehicleProperty;
import android.hardware.automotive.vehicle.V2_0.VehicleApPowerState;
import android.os.PowerManager;
import android.os.RemoteException;
import android.util.Log;
import android.content.Context;
import static android.content.Context.POWER_SERVICE;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import org.mockito.*;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;
import android.app.Instrumentation;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.support.test.filters.SmallTest;
import android.support.test.rule.ActivityTestRule;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Calendar;


@SmallTest
@RunWith(AndroidJUnit4.class)
public class BrightnessServiceTest{
    private Context mTargetContext;
    private String TAG = "Brightness.AP_POWER.TEST";
    PowerManager mPowerManager = null;
    IVehicle mVehicle = null;
    CSDConsumerManager _CSDConsumerManager = null;
    CSDConsumerManager.VehicleCallback _CSDConsumerManagermInternalCallback = null;
    @Before
    public void setUp() throws Exception {
        Instrumentation instrumentation = InstrumentationRegistry.getInstrumentation();
        mTargetContext = instrumentation.getTargetContext();
        mVehicle = mock(IVehicle.Stub.class);
        setupVehicleSetMock();
    }
    private void setupCSDConsumerManager() throws NoSuchFieldException, IllegalAccessException {
        mPowerManager = (PowerManager) mTargetContext.getSystemService(POWER_SERVICE);
        Log.v(TAG,"Creating CSDConsumerManager");
        _CSDConsumerManager = new CSDConsumerManager(mVehicle, mPowerManager);
        Log.v(TAG,"Changing access level for mInternalCallback in CSDConsumerManager");
        Field field = _CSDConsumerManager.getClass().getDeclaredField("mInternalCallback");
        field.setAccessible(true);
        Log.v(TAG,"Getting reference for mInternalCallback in CSDConsumerManager");
        _CSDConsumerManagermInternalCallback = (CSDConsumerManager.VehicleCallback) field.get(_CSDConsumerManager);
    }
    /**
     * This function sets up vehicle hal get function for AP_POWER_STATE and IGNITION_STATE.
     * @param powerState is the powerstate the get function should return when getting AP_POWER_STATE
     * property.
     */
    private void setupVehicleGetMock(int powerState) throws RemoteException {
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public Object answer(InvocationOnMock invocation) throws Throwable {
                Log.v(TAG,"getFunctionInvoked");

                VehiclePropValue wantedprop = (VehiclePropValue)invocation.getArguments()[0];
                VehiclePropValue returnprop = new VehiclePropValue();
                switch (wantedprop.prop) {
                    case VehicleProperty.IGNITION_STATE:
                        returnprop.prop = VehicleProperty.IGNITION_STATE;
                        returnprop.value.int32Values.add(VehicleIgnitionState.OFF);
                        break;
                    case VehicleProperty.AP_POWER_STATE:
                        returnprop.prop = VehicleProperty.AP_POWER_STATE;
                        returnprop.value.int32Values.add(powerState);
                    default:
                        break;
                }
                returnprop.areaId = 0;
                returnprop.timestamp = Calendar.getInstance().getTimeInMillis();
                ((IVehicle.getCallback)invocation.getArguments()[1]).onValues(0,returnprop);
                return null;
            }
        }).when(mVehicle).get(any(VehiclePropValue.class),any(IVehicle.getCallback.class));
    }
    /**
     * This function sets up vehicle hal set function and will always return a zero as success.
     */
    private void setupVehicleSetMock() throws RemoteException {
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public Object answer(InvocationOnMock invocation) throws Throwable {
                return 0;
            }
        }).when(mVehicle).set(any(VehiclePropValue.class));
    }

    /**
     * Test verifies that the CSDConsumerManager react to AP_POWER_STATE Change
     * Arrange : Set AP_POWER_STATE ON_FULL
     * Act: Call OnChangeCallback with AP_POWER_STATE.ON_DISP_OFF
     * Assert : Vehicle hal receives VehicleApPowerSetState.DISPLAY_OFF
     */
    @Test
    public void ApPowerChangeTestON_DISP_OFF(){
        Log.v(TAG,"Start ApPowerChangeTest");
        try {

            setupVehicleGetMock(VehicleApPowerState.ON_FULL);
            setupCSDConsumerManager();
            Mockito.reset(mVehicle);

            ArrayList<VehiclePropValue> propValues = new ArrayList<>();
            VehiclePropValue prop = new VehiclePropValue();
            prop.prop = VehicleProperty.AP_POWER_STATE;
            prop.value.int32Values.add(VehicleApPowerState.ON_DISP_OFF);
            prop.areaId = 0;
            prop.timestamp = Calendar.getInstance().getTimeInMillis();
            propValues.add(prop);

            VehiclePropValue wantedsetprop = new VehiclePropValue();
            wantedsetprop.prop = VehicleProperty.AP_POWER_STATE;
            wantedsetprop.value.int32Values.add(VehicleApPowerSetState.DISPLAY_OFF);
            wantedsetprop.areaId = VehicleArea.GLOBAL;

            _CSDConsumerManagermInternalCallback.onPropertyEvent(propValues);
            Log.v(TAG,"ON_DISP_OFF Injected");
            verify(mVehicle,times(1)).set(wantedsetprop);

        } catch (RemoteException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
        Log.v(TAG,"End ApPowerChangeTestON_DISP_OFF");

    }
    /**
     * Test verifies that the CSDConsumerManager react to AP_POWER_STATE Change
     * Arrange : Set AP_POWER_STATE.ON_FULL
     * Act: Call OnChangeCallback with AP_POWER_STATE.ON_DISP_OFF
     * Assert : Vehicle hal receives VehicleApPowerSetState.DISPLAY_OFF.
     */
    @Test
    public void ApPowerChangeTestON_FULL(){
        Log.v(TAG,"Start ApPowerChangeTest");
        try {

            setupVehicleGetMock(VehicleApPowerState.ON_DISP_OFF);
            setupCSDConsumerManager();
            Mockito.reset(mVehicle);

            ArrayList<VehiclePropValue> propValues = new ArrayList<>();
            VehiclePropValue prop = new VehiclePropValue();
            prop.prop = VehicleProperty.AP_POWER_STATE;
            prop.value.int32Values.add(VehicleApPowerState.ON_FULL);
            prop.areaId = 0;
            prop.timestamp = Calendar.getInstance().getTimeInMillis();
            propValues.add(prop);

            _CSDConsumerManagermInternalCallback.onPropertyEvent(propValues);
            Log.v(TAG,"DISPLAY_ON Injected");

            VehiclePropValue wantedsetprop = new VehiclePropValue();
            wantedsetprop.prop = VehicleProperty.AP_POWER_STATE;
            wantedsetprop.value.int32Values.add(VehicleApPowerSetState.DISPLAY_ON);
            wantedsetprop.areaId = VehicleArea.GLOBAL;

            verify(mVehicle,times(1)).set(wantedsetprop);
        } catch (RemoteException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
        Log.v(TAG,"End ApPowerChangeTestON_FULL");
    }

}