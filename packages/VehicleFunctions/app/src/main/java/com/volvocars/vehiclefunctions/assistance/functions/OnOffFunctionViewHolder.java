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

        // View state handling
        LiveData<FunctionState> enabled = Transformations.switchMap(mFunction, OnOffFunction::getFunctionState);
        enabled.observeForever(buttonState -> {
            switch (buttonState){
                case ENABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mOnButton.setVisibility(View.VISIBLE);
                    mOffButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(true);
                    mOnButton.setEnabled(true);
                    mOffButton.setEnabled(true);
                    break;
                case DISABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mOnButton.setVisibility(View.VISIBLE);
                    mOffButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mOnButton.setEnabled(false);
                    mOffButton.setEnabled(false);
                    break;
                case INVISIBLE:
                    itemView.setVisibility(View.GONE);
                    mOnButton.setVisibility(View.GONE);
                    mOffButton.setVisibility(View.GONE);
                    break;
                case ERROR:
                    // TODO: found out what to show
                    itemView.setVisibility(View.VISIBLE);
                    mOnButton.setVisibility(View.VISIBLE);
                    mOffButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mOnButton.setEnabled(false);
                    mOffButton.setEnabled(false);

            }

            mOnButton.setId(mFunction.getValue().getOnButtonId());
            mOffButton.setId(mFunction.getValue().getOffButtonId());
        });

        mOnButton.setOnClickListener(v -> mFunction.getValue().setState(true));
        mOffButton.setOnClickListener(v -> mFunction.getValue().setState(false));
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        mFunction.setValue((OnOffFunction) function);
    }
}
