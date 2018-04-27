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
public class OneButtonFunction extends Function {
    private static final int COLUMNS_OCCUPIED = 1;

    private final Delegate mDelegate;
    private final MutableLiveData<Boolean> mStateLiveData = new MutableLiveData<>();
    private final MutableLiveData<Boolean> mEnabledLiveData = new MutableLiveData<>();
    private final int mButtonId;

    public OneButtonFunction(CharSequence title, Delegate delegate, int onButtonId) {
        super(title, R.layout.vh_function_onebutton, COLUMNS_OCCUPIED);
        mDelegate = delegate;
        mButtonId = onButtonId;
        mDelegate.setListener(stateChangedListener);
        mDelegate.onSetInitialState();
        mEnabledLiveData.postValue(false);
    }

    public LiveData<Boolean> getState() {
        return mStateLiveData;
    }

    public LiveData<Boolean> getEnabled() {
        return mEnabledLiveData;
    }

    public int getOnButtonId() { return mButtonId; }

    public void setState(boolean state) {
        CompletableFuture.runAsync(() -> mDelegate.onUserChangedState(state));
    }

    private final Delegate.StateChangedListener stateChangedListener =
            new Delegate.StateChangedListener() {
        @Override
        public void stateChanged(boolean state) {
            mStateLiveData.postValue(state);
        }

        @Override
        public void enabledChanged(boolean enabled) {
            mEnabledLiveData.postValue(enabled);
        }
    };


    public static abstract class Delegate {
        private StateChangedListener mListener;

        protected abstract void onUserChangedState(boolean value);
        protected abstract void onSetInitialState();

        protected void setUiState(boolean state) {
            mListener.stateChanged(state);
        }

        protected void setUiEnabled(boolean enabled) {
            mListener.enabledChanged(enabled);
        }

        private void setListener(StateChangedListener listener) {
            this.mListener = listener;
        }

        private interface StateChangedListener {
            void stateChanged(boolean state);
            void enabledChanged(boolean enabled);
        }
    }
}
