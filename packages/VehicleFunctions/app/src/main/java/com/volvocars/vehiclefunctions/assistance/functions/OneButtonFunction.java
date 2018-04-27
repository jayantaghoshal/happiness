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

    private final VHalDelegate mVHalDelegate;
    private final MutableLiveData<Boolean> mStateLiveData = new MutableLiveData<>();
    private final MutableLiveData<FunctionViewHolder.FunctionState> buttonStateLiveData = new MutableLiveData<>();
    private final int mButtonId;

    public OneButtonFunction(CharSequence title, VHalDelegate VHalDelegate, int onButtonId) {
        super(title, R.layout.vh_function_onebutton, COLUMNS_OCCUPIED);
        mVHalDelegate = VHalDelegate;
        mButtonId = onButtonId;
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
        return mButtonId;
    }

    public void setState(boolean state) {
        CompletableFuture.runAsync(() -> mVHalDelegate.onUserChangedValue(state));
    }

    private final VHalDelegate.StateChangedListener<Boolean> stateChangedListener =
            new VHalDelegate.StateChangedListener<Boolean>() {

                @Override
                public void valueChanged(Boolean value) {
                    mStateLiveData.postValue(value);
                }

                @Override
                public void buttonStateChanged(FunctionViewHolder.FunctionState buttonState) {
                    buttonStateLiveData.postValue(buttonState);
                }
            };
}
