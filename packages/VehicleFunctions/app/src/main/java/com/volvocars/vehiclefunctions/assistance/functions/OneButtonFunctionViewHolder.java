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
 * ViewHolder for a OneButtonFunction in a recycler view.
 * Updates the view with data specific to a OneButtonFunction.
 */
public class OneButtonFunctionViewHolder extends FunctionViewHolder {
    private final MutableLiveData<OneButtonFunction> mFunction = new MutableLiveData<>();

    private final Button mButton;

    public OneButtonFunctionViewHolder(View itemView) {
        super(itemView);

        mButton = itemView.findViewById(R.id.button_on);

        LiveData<Boolean> state = Transformations.switchMap(mFunction, OneButtonFunction::getState);
        state.observeForever(state1 -> {
            mButton.setTextColor(!state1 ? itemView.getResources().getColor(R.color.colorRedLight)
                    : itemView.getResources().getColor(R.color.colorGreenLight));
            mButton.setText(!state1 ? "Off" : "On");
        });

        LiveData<Boolean> enabled = Transformations.switchMap(mFunction, OneButtonFunction::getEnabled);
        enabled.observeForever(enabled1 -> {
            itemView.setEnabled(enabled1);
            mButton.setEnabled(enabled1);
            mButton.setId(mFunction.getValue().getOnButtonId());
        });

        mButton.setOnClickListener(v -> mFunction.getValue().setState(true));
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        mFunction.setValue((OneButtonFunction)function);
    }
}
