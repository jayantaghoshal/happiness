/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.functions;

import android.arch.core.executor.testing.InstantTaskExecutorRule;

import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunction;

import org.junit.Rule;
import org.junit.Test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;


public class OnOffFunctionTests {
    private boolean callbackState = false;

    @Rule
    public InstantTaskExecutorRule instantExecutorRule = new InstantTaskExecutorRule();

    @Test
    public void getEnabled_should_be_false_by_default() {
        OnOffFunction onOffFunction = new OnOffFunction("", new ImmediateUpdateOnOffDelegate());

        boolean defaultEnabled = onOffFunction.getEnabled().getValue();

        assertFalse(defaultEnabled);
    }

    @Test
    public void getState_should_be_null_by_default() {
        OnOffFunction onOffFunction = new OnOffFunction("", new ImmediateUpdateOnOffDelegate());

        Object defaultState = onOffFunction.getState().getValue();

        assertNull(defaultState);
    }

    @Test
    public void setState_should_cause_the_getState_LiveData_to_update() {
        OnOffFunction onOffFunction = new OnOffFunction("", new ImmediateUpdateOnOffDelegate());
        onOffFunction.getState().observeForever(value -> callbackState = value);

        onOffFunction.setState(true);

        assertTrue(callbackState);
    }


    /**
     * Utilities
     */
    private static class ImmediateUpdateOnOffDelegate extends OnOffFunction.Delegate {

        @Override
        protected void onUserChangedState(boolean value) {
            setUiState(value);
        }

        @Override
        protected void onSetInitialState() {

        }
    }
}