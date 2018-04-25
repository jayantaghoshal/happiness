/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentPagerAdapter;

import com.volvocars.vehiclefunctions.assistance.AssistanceFragment;
import com.volvocars.vehiclefunctions.driving.DrivingFragment;
import com.volvocars.vehiclefunctions.status.StatusFragment;

/**
 * FragmentPagerAdapter for Vehicle Functions
 */
public class VehicleFunctionsPagerAdapter extends FragmentPagerAdapter {

    public VehicleFunctionsPagerAdapter(FragmentActivity activity) {
        super(activity.getSupportFragmentManager());
    }

    @Override
    public Fragment getItem(int position) {

        switch (position) {
            case 0:
                return DrivingFragment.instantiate();
            case 1:
                return AssistanceFragment.instantiate();
            case 2:
                return StatusFragment.instantiate();
            default:
                // "This should never happen"
                throw new IndexOutOfBoundsException();
        }
    }

    @Override
    public int getCount() {
        return 3;
    }
}
