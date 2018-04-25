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
 * Represents a function which can be in one of three possible states, 0, 1, or 2.
 */
public class ThreeStateFunction extends Function {
    private static final int COLUMNS_OCCUPIED = 2;

    private final CharSequence mState1Name;
    private final CharSequence mState2Name;
    private final CharSequence mState3Name;
    private final Delegate mDelegate;
    private final MutableLiveData<Integer> mStateLiveData = new MutableLiveData<>();
    private final MutableLiveData<Boolean> mEnabledLiveData = new MutableLiveData<>();
    private final int mState1ButtonId;
    private final int mState2ButtonId;
    private final int mState3ButtonId;

    public ThreeStateFunction(CharSequence title, CharSequence state1Name, CharSequence state2Name,
                              CharSequence state3Name, Delegate delegate, int state1ButtonId, int state2ButtonId, int state3ButtonId) {
        super(title, R.layout.vh_function_threestate, COLUMNS_OCCUPIED);
        mState1Name = state1Name;
        mState2Name = state2Name;
        mState3Name = state3Name;
        mDelegate = delegate;
        mDelegate.setListener(stateChangedListener);
        mDelegate.onSetInitialState();
        mState1ButtonId = state1ButtonId;
        mState2ButtonId = state2ButtonId;
        mState3ButtonId = state3ButtonId;
        mEnabledLiveData.setValue(false);
    }

    public CharSequence getState1Name() {
        return mState1Name;
    }

    public CharSequence getState2Name() {
        return mState2Name;
    }

    public CharSequence getState3Name() {
        return mState3Name;
    }

    public LiveData<Integer> getState() {
        return mStateLiveData;
    }

    public LiveData<Boolean> getEnabled() {
        return mEnabledLiveData;
    }

    public int getState1ButtonId() { return mState1ButtonId; }

    public int getState2ButtonId() { return mState2ButtonId; }

    public int getState3ButtonId() { return mState3ButtonId; }

    public void setState(int state) {
        CompletableFuture.runAsync(() -> mDelegate.onUserChangedState(state));
    }

    private final Delegate.StateChangedListener stateChangedListener = new Delegate.StateChangedListener() {
        @Override
        public void stateChanged(int state) {
            mStateLiveData.postValue(state);
        }

        @Override
        public void enabledChanged(boolean enabled) {
            mEnabledLiveData.postValue(enabled);
        }
    };


    public static abstract class Delegate {
        private StateChangedListener mListener;

        protected abstract void onUserChangedState(int value);

        protected abstract void onSetInitialState();

        protected void setUiState(int value) {
            mListener.stateChanged(value);
        }

        protected void setUiEnabled(boolean enabled) {
            mListener.enabledChanged(enabled);
        }

        private void setListener(StateChangedListener listener) {
            this.mListener = listener;
        }

        private interface StateChangedListener {
            void stateChanged(int value);
            void enabledChanged(boolean enabled);
        }
    }
}