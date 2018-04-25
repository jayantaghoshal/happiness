/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

/**
 * Data for an individual vehicle function
 * TODO make this class abstract
 */
public class Function {
    private final CharSequence mTitle;
    private final int mLayoutId;
    private int mSlotsOccupied;

    /**
     * @param title The name to be displayed for this function.
     * @param layoutId The layout file id to show this function in a recyclerview.
     * @param slotsOccupied Functions occupy different amounts of UI space. This is the number of columns to occupy.
     */
    public Function(CharSequence title, int layoutId, int slotsOccupied) {
        mTitle = title;
        mLayoutId = layoutId;
        mSlotsOccupied = slotsOccupied;
    }

    public CharSequence getTitle() {
        return mTitle;
    }

    public int getSlotsOccupied() {
        return mSlotsOccupied;
    }

    public int getLayoutId() {
        return mLayoutId;
    }
}
