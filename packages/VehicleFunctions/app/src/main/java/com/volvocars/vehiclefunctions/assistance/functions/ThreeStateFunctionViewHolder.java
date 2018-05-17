/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.arch.lifecycle.Transformations;
import android.view.View;
import android.widget.Button;

import com.volvocars.vehiclefunctions.R;

/**
 * ViewHolder for an ThreeStateFunction in a recycler view.
 * Updates the view with data specific to a ThreeStateFunction.
 */
public class ThreeStateFunctionViewHolder extends FunctionViewHolder {
    private final MutableLiveData<ThreeStateFunction> mFunction = new MutableLiveData<>();

    private static final int MODE_1_DISABLED = 1;
    private static final int MODE_2_DISABLED = 2;
    private static final int MODE_3_DISABLED = 3;

    private static final int STATE_1 = 0;
    private static final int STATE_2 = 1;
    private static final int STATE_3 = 2;

    private final Button mButton1;
    private final Button mButton2;
    private final Button mButton3;

    public ThreeStateFunctionViewHolder(View itemView) {
        super(itemView);

        mButton1 = itemView.findViewById(R.id.button_1);
        mButton2 = itemView.findViewById(R.id.button_2);
        mButton3 = itemView.findViewById(R.id.button_3);

        int colorGreen = itemView.getResources().getColor(R.color.colorGreenLight);
        int colorRed = itemView.getResources().getColor(R.color.colorRedLight);

        LiveData<Integer> state = Transformations.switchMap(mFunction, ThreeStateFunction::getState);
        state.observeForever(state1 -> {
            mButton1.setTextColor(state1 == STATE_1 ? colorGreen : colorRed);
            mButton2.setTextColor(state1 == STATE_2 ? colorGreen : colorRed);
            mButton3.setTextColor(state1 == STATE_3 ? colorGreen : colorRed);
        });

        LiveData<FunctionState> enabled = Transformations.switchMap(mFunction, ThreeStateFunction::getFunctionState);
        enabled.observeForever(buttonState -> {
            switch (buttonState){
                case ENABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mButton1.setVisibility(View.VISIBLE);
                    mButton2.setVisibility(View.VISIBLE);
                    mButton3.setVisibility(View.VISIBLE);
                    itemView.setEnabled(true);
                    mButton1.setEnabled(true);
                    mButton2.setEnabled(true);
                    mButton3.setEnabled(true);
                    break;
                case DISABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mButton1.setVisibility(View.VISIBLE);
                    mButton2.setVisibility(View.VISIBLE);
                    mButton3.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mButton1.setEnabled(false);
                    mButton2.setEnabled(false);
                    mButton3.setEnabled(false);
                    break;
                case INVISIBLE:
                    itemView.setVisibility(View.GONE);
                    mButton1.setVisibility(View.GONE);
                    mButton2.setVisibility(View.GONE);
                    mButton3.setVisibility(View.GONE);
                    break;
                case ERROR:
                    // TODO: found out what to show
                    itemView.setVisibility(View.VISIBLE);
                    mButton1.setVisibility(View.VISIBLE);
                    mButton2.setVisibility(View.VISIBLE);
                    mButton3.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mButton1.setEnabled(false);
                    mButton2.setEnabled(false);
                    mButton3.setEnabled(false);
                    break;
            }
            removeButtonFromView(mFunction.getValue().getDisabledMode().getValue());
        });

        LiveData<Integer> disabledMode = Transformations.switchMap(mFunction, ThreeStateFunction::getDisabledMode);
        disabledMode.observeForever(disabledMode1 -> {
            removeButtonFromView(disabledMode.getValue());
            mButton1.setId(mFunction.getValue().getState1ButtonId());
            mButton2.setId(mFunction.getValue().getState2ButtonId());
            mButton3.setId(mFunction.getValue().getState3ButtonId());
        });

        mButton1.setOnClickListener(v -> mFunction.getValue().setState(STATE_1));
        mButton2.setOnClickListener(v -> mFunction.getValue().setState(STATE_2));
        mButton3.setOnClickListener(v -> mFunction.getValue().setState(STATE_3));
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        ThreeStateFunction threeStateFunction = (ThreeStateFunction) function;
        mFunction.setValue(threeStateFunction);

        mButton1.setText(threeStateFunction.getState1Name());
        mButton2.setText(threeStateFunction.getState2Name());
        mButton3.setText(threeStateFunction.getState3Name());
    }

    private void removeButtonFromView(int disabledMode) {
        switch (disabledMode) {
            case MODE_1_DISABLED:
                mButton1.setVisibility(View.GONE);
                break;
            case MODE_2_DISABLED:
                mButton2.setVisibility(View.GONE);
                break;
            case MODE_3_DISABLED:
                mButton3.setVisibility(View.GONE);
                break;
            default:
                break;
        }
    }
}
