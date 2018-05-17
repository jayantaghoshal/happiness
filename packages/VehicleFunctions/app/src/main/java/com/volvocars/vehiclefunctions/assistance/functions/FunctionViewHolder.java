/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.TextView;

import com.volvocars.vehiclefunctions.R;

/**
 * ViewHolder base class for a function in a recycler view.
 * Updates the view with data common to all functions e.g. title.
 */
public class FunctionViewHolder extends RecyclerView.ViewHolder {
    private final TextView mTitleTextView;

    public enum FunctionState {
        ERROR, ENABLED, DISABLED, INVISIBLE
    }

    public FunctionViewHolder(View itemView) {
        super(itemView);
        mTitleTextView = itemView.findViewById(R.id.textView_title);
    }

    public void bind(Function function) {
        mTitleTextView.setText(function.getTitle());
    }
}
