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

        // View state handling
        LiveData<FunctionState> enabled = Transformations.switchMap(mFunction, OneButtonFunction::getFunctionState);
        enabled.observeForever(buttonState -> {
            switch (buttonState){
                case ENABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(true);
                    mButton.setEnabled(true);
                    break;
                case DISABLED:
                    itemView.setVisibility(View.VISIBLE);
                    mButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mButton.setEnabled(false);
                    break;
                case INVISIBLE:
                    itemView.setVisibility(View.GONE);
                    mButton.setVisibility(View.GONE);
                    itemView.setEnabled(false);
                    mButton.setEnabled(false);
                    break;
                case ERROR:
                    // TODO: found out what to show
                    itemView.setVisibility(View.VISIBLE);
                    mButton.setVisibility(View.VISIBLE);
                    itemView.setEnabled(false);
                    mButton.setEnabled(false);
                    break;
            }
            mButton.setId(mFunction.getValue().getOnButtonId());
        });

        mButton.setOnClickListener(v -> mFunction.getValue().setState(true));
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        mFunction.setValue((OneButtonFunction) function);
    }
}
