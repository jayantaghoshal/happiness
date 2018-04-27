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
 * Therefore uses a boolean for the current state.
 */
public class OnOffFunction extends Function {
    private static final int COLUMNS_OCCUPIED = 1;

    private final VHalDelegate mVHalDelegate;
    private final MutableLiveData<Boolean> mStateLiveData = new MutableLiveData<>();
    private final MutableLiveData<FunctionViewHolder.FunctionState> buttonStateLiveData = new MutableLiveData<>();
    private final int mOnButtonId;
    private final int mOffButtonId;

    public OnOffFunction(CharSequence title, VHalDelegate VHalDelegate, int onButtonId, int offButtonId) {
        super(title, R.layout.vh_function_onoff, COLUMNS_OCCUPIED);
        mVHalDelegate = VHalDelegate;
        mOnButtonId = onButtonId;
        mOffButtonId = offButtonId;
        mVHalDelegate.setListener(stateChangedListener);
        mVHalDelegate.onSetInitialState();
    }

    public LiveData<Boolean> getState() {
        return mStateLiveData;
    }

    public LiveData<FunctionViewHolder.FunctionState> getFunctionState() {
        return buttonStateLiveData;
    }

    public int getOnButtonId() {
        return mOnButtonId;
    }

    public int getOffButtonId() {
        return mOffButtonId;
    }

    public void setState(boolean state) {
        CompletableFuture.runAsync(() -> mVHalDelegate.onUserChangedValue(state));
    }

    private final VHalDelegate.StateChangedListener<Boolean> stateChangedListener =
            new VHalDelegate.StateChangedListener<Boolean>() {
                @Override
                public void valueChanged(Boolean state) {
                    mStateLiveData.postValue(state);
                }

                @Override
                public void buttonStateChanged(FunctionViewHolder.FunctionState buttonState) {
                    buttonStateLiveData.postValue(buttonState);
                }
            };
}
