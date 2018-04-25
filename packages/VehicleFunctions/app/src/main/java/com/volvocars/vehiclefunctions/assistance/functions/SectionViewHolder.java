/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.volvocars.vehiclefunctions.R;

/**
 * ViewHolder for an Section in a recycler view.
 * Sets the margins to create space between sections without offsetting the first Section.
 */
public class SectionViewHolder extends FunctionViewHolder {
    private final int margin;
    private final int extraMargin;

    public SectionViewHolder(View itemView) {
        super(itemView);
        margin = (int) itemView.getResources().getDimension(R.dimen.function_margin);
        extraMargin = (int) itemView.getResources().getDimension(R.dimen.function_margin_extra);
    }

    @Override
    public void bind(Function function) {
        super.bind(function);
        RecyclerView.LayoutParams layoutParams = (RecyclerView.LayoutParams) itemView.getLayoutParams();

        if (getAdapterPosition() == 0) {
            layoutParams.setMargins(margin, margin, margin, margin);
        } else {
            layoutParams.setMargins(margin, extraMargin, margin, margin);
        }

        itemView.setLayoutParams(layoutParams);
    }
}
