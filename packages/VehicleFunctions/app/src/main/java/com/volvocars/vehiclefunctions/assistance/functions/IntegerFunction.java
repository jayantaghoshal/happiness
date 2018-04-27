/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;

import com.volvocars.vehiclefunctions.R;

import java.util.concurrent.CompletableFuture;

/**
 * Represents a function which can be in either an On or Off state.
 * This function is connected to other OneButtonFunctions. Only one of the connected
 * OneButtonFunctions can be in the state 'On'. The rest of them shall be set to 'Off'.
 */
public class IntegerFunction extends Function {
    private static final int COLUMNS_OCCUPIED = 1;

    private final Delegate mDelegate;
    private final MutableLiveData<Boolean> mEnabledLiveData = new MutableLiveData<>();
    private final MutableLiveData<Integer> mEnabledLiveValue = new MutableLiveData<>();
    private final int incButtonId;
    private final int IntTextId;
    private final int decButtonId;
    private final MutableLiveData<Integer> minimum = new MutableLiveData<>();;
    private final MutableLiveData<Integer> maximum = new MutableLiveData<>();;
    private final MutableLiveData<Integer> incDecStepSize = new MutableLiveData<>();;

    public IntegerFunction(CharSequence title, Delegate delegate, int plusButtonId, int textViewId, int minusButtonId ,int min,int max,int resolution ) {
        super(title, R.layout.vh_function_plusminus, COLUMNS_OCCUPIED);
        mDelegate = delegate;
        incButtonId = plusButtonId;
        IntTextId = textViewId;
        decButtonId = minusButtonId;
        minimum.postValue(min);
        maximum.postValue(max);
        incDecStepSize.postValue(resolution);

        mDelegate.setListener(stateChangedListener);
        mDelegate.onSetInitialState();
        mEnabledLiveData.postValue(false);
    }

    public LiveData<Integer> getTextValue() {
        return mEnabledLiveValue;
    }

    public LiveData<Boolean> getEnabled() {
        return mEnabledLiveData;
    }

    public LiveData<Integer> getMaximum() {
        return maximum;
    }

    public LiveData<Integer> getMinimum() {
        return minimum;
    }

    public LiveData<Integer> getResolution() {
        return incDecStepSize;
    }

    public int getPlusButtonId() { return incButtonId; }

    public int getTextViewId() { return IntTextId; }

    public int getMinusButtonId() { return decButtonId; }

    public void setValue(int value) {
        CompletableFuture.runAsync(() -> mDelegate.onUserChangedValue(value));
    }

    private final Delegate.StateChangedListener stateChangedListener =
            new Delegate.StateChangedListener() {

        @Override
        public void valueChanged(int value) {
            mEnabledLiveValue.postValue(value);
        }

        @Override
        public void enabledChanged(boolean enabled) {
            mEnabledLiveData.postValue(enabled);
        }
    };


    public static abstract class Delegate {
        private StateChangedListener mListener;

        protected abstract void onUserChangedValue(int value);

        protected abstract void onSetInitialState();

        protected void setUiValue(int value) {
            mListener.valueChanged(value);
        }

        protected void setUiEnabled(boolean enabled) {
            mListener.enabledChanged(enabled);
        }

        private void setListener(StateChangedListener listener) {
            this.mListener = listener;
        }

        private interface StateChangedListener {
            void valueChanged(int value);
            void enabledChanged(boolean enabled);
        }
    }
}
