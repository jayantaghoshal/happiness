/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.functions;

import android.arch.core.executor.testing.InstantTaskExecutorRule;

import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunction;

import org.junit.Rule;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;

public class ThreeStateFunctionTests {
    private int callbackState = 0;

    @Rule
    public InstantTaskExecutorRule instantExecutorRule = new InstantTaskExecutorRule();

    @Test
    public void getEnabled_should_be_false_by_default() {
        ThreeStateFunction threeStateFunction = new ThreeStateFunction("", "",
                "", "", new ImmediateUpdateThreeStateDelegate());

        boolean defaultEnabled = threeStateFunction.getEnabled().getValue();

        assertFalse(defaultEnabled);
    }

    @Test
    public void getState_should_be_null_by_default() {
        ThreeStateFunction threeStateFunction = new ThreeStateFunction("", "",
                "", "", new ImmediateUpdateThreeStateDelegate());

        Object defaultState = threeStateFunction.getState().getValue();

        assertNull(defaultState);
    }

    @Test
    public void setState_should_cause_the_getState_LiveData_to_update() {
        int newState = 2;
        ThreeStateFunction threeStateFunction = new ThreeStateFunction("", "",
                "", "", new ImmediateUpdateThreeStateDelegate());
        threeStateFunction.getState().observeForever(value -> callbackState = value);

        threeStateFunction.setState(newState);

        assertEquals(newState, callbackState);
    }


    /**
     * Utilities
     */
    private static class ImmediateUpdateThreeStateDelegate extends ThreeStateFunction.Delegate {

        @Override
        protected void onUserChangedState(int value) {
            setUiState(value);
        }

        @Override
        protected void onSetInitialState() {

        }
    }
}