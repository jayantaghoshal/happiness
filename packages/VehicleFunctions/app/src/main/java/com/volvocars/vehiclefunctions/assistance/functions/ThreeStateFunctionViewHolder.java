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

    private final Button mButton1;
    private final Button mButton2;
    private final Button mButton3;

    public ThreeStateFunctionViewHolder(View itemView) {
        super(itemView);

        mButton1 = itemView.findViewById(R.id.button_1);
        mButton2 = itemView.findViewById(R.id.button_2);
        mButton3 = itemView.findViewById(R.id.button_3);

        LiveData<Integer> state = Transformations.switchMap(mFunction, ThreeStateFunction::getState);
        state.observeForever(state1 -> {
            mButton1.setTextColor(state1 == 0 ? itemView.getResources().getColor(R.color.colorGreenLight)
                    : itemView.getResources().getColor(R.color.colorRedLight));
            mButton2.setTextColor(state1 == 1 ? itemView.getResources().getColor(R.color.colorGreenLight)
                    : itemView.getResources().getColor(R.color.colorRedLight));
            mButton3.setTextColor(state1 == 2 ? itemView.getResources().getColor(R.color.colorGreenLight)
                    : itemView.getResources().getColor(R.color.colorRedLight));
        });

        LiveData<Boolean> enabled = Transformations.switchMap(mFunction, ThreeStateFunction::getEnabled);
        enabled.observeForever(enabled1 -> {
            itemView.setEnabled(enabled1);
            mButton1.setEnabled(enabled1);
            mButton2.setEnabled(enabled1);
            mButton3.setEnabled(enabled1);
            mButton1.setId(mFunction.getValue().getState1ButtonId());
            mButton2.setId(mFunction.getValue().getState2ButtonId());
            mButton3.setId(mFunction.getValue().getState3ButtonId());
        });

        mButton1.setOnClickListener(v -> mFunction.getValue().setState(0));

        mButton2.setOnClickListener(v -> mFunction.getValue().setState(1));

        mButton3.setOnClickListener(v -> mFunction.getValue().setState(2));
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
}
