/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.arch.lifecycle.Transformations;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.volvocars.vehiclefunctions.R;

/**
 * ViewHolder for an OnOffFunction in a recycler view.
 * Updates the view with data specific to an OnOffFunction.
 */
public class IntegerFunctionViewHolder extends FunctionViewHolder {
    private final static String TAG = IntegerFunctionViewHolder.class.getSimpleName();
    private final MutableLiveData<IntegerFunction> mFunction = new MutableLiveData<>();

    private final Button plusButton;
    private final TextView textView;
    private final Button minusButton;
    private int minimum;
    private int maximum;
    private int incDecStepSize;


    public IntegerFunctionViewHolder(View itemView) {
        super(itemView);

        plusButton = itemView.findViewById(R.id.offset_button_plus);
        textView = itemView.findViewById(R.id.offset_text);
        minusButton = itemView.findViewById(R.id.offset_button_minus);

        Transformations.switchMap(mFunction, IntegerFunction::getMaximum).observeForever(newMaximum ->
                {
                    maximum = newMaximum;
                }
        );
        Transformations.switchMap(mFunction, IntegerFunction::getMinimum).observeForever(newMinimum ->
                {
                    minimum = newMinimum;
                }
        );
        Transformations.switchMap(mFunction, IntegerFunction::getResolution).observeForever(newResolution ->
                {
                    incDecStepSize = newResolution;
                }
        );

        mFunction.observeForever(newFunction ->
                {
                    plusButton.setId(newFunction.getPlusButtonId());
                    textView.setId(newFunction.getTextViewId());
                    minusButton.setId(newFunction.getMinusButtonId());
                }
        );

        LiveData<Boolean> enabled = Transformations.switchMap(mFunction, IntegerFunction::getEnabled);
        enabled.observeForever(enabled1 -> {
            itemView.setEnabled(enabled1);
            plusButton.setEnabled(enabled1);
            textView.setEnabled(enabled1);
            minusButton.setEnabled(enabled1);
        });

        LiveData<Integer> value = Transformations.switchMap(mFunction, IntegerFunction::getTextValue);
        value.observeForever(value1 -> {
            textView.setText(Integer.toString(value1));
        });

        plusButton.setOnClickListener(v -> {
                    if (Integer.parseInt(textView.getText().toString()) < maximum) {
                        mFunction.getValue().setValue(Integer.parseInt(textView.getText().toString()) + incDecStepSize);
                    }
                }
        );
        minusButton.setOnClickListener(v -> {
                    if (Integer.parseInt(textView.getText().toString()) > minimum) {
                        mFunction.getValue().setValue(Integer.parseInt(textView.getText().toString()) - incDecStepSize);
                    }
                }
        );
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        mFunction.setValue((IntegerFunction) function);
    }
}
