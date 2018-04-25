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
 * ViewHolder for an OnOffFunction in a recycler view.
 * Updates the view with data specific to an OnOffFunction.
 */
public class OnOffFunctionViewHolder extends FunctionViewHolder {
    private final MutableLiveData<OnOffFunction> mFunction = new MutableLiveData<>();

    private final Button mOnButton;
    private final Button mOffButton;

    public OnOffFunctionViewHolder(View itemView) {
        super(itemView);

        mOnButton = itemView.findViewById(R.id.button_on);
        mOffButton = itemView.findViewById(R.id.button_off);

        LiveData<Boolean> state = Transformations.switchMap(mFunction, OnOffFunction::getState);
        state.observeForever(state1 -> {
            mOnButton.setTextColor(!state1 ? itemView.getResources().getColor(R.color.colorRedLight)
                    : itemView.getResources().getColor(R.color.colorGreenLight));
            mOffButton.setTextColor(state1 ? itemView.getResources().getColor(R.color.colorRedLight)
                    : itemView.getResources().getColor(R.color.colorGreenLight));
        });

        LiveData<Boolean> enabled = Transformations.switchMap(mFunction, OnOffFunction::getEnabled);
        enabled.observeForever(enabled1 -> {
            itemView.setEnabled(enabled1);
            mOnButton.setEnabled(enabled1);
            mOffButton.setEnabled(enabled1);
            mOnButton.setId(mFunction.getValue().getOnButtonId());
            mOffButton.setId(mFunction.getValue().getOffButtonId());
        });

        mOnButton.setOnClickListener(v -> mFunction.getValue().setState(true));

        mOffButton.setOnClickListener(v -> mFunction.getValue().setState(false));
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        mFunction.setValue((OnOffFunction)function);
    }
}
