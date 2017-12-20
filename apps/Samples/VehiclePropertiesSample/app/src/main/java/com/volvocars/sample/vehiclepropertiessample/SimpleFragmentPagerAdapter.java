/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.sample.vehiclepropertiessample;

import android.content.Context;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class SimpleFragmentPagerAdapter extends FragmentPagerAdapter {

    private Context mContext;

    public SimpleFragmentPagerAdapter(Context context, FragmentManager fm) {
        super(fm);
        mContext = context;
    }


    // This determines the number of tabs
    @Override
    public int getCount() {
        return 3;
    }

    // This determines the title for each tab
    @Override
    public CharSequence getPageTitle(int position) {
        // Generate title based on item position
        switch (position) {
            case 0:
                return mContext.getString(R.string.home);
            case 1:
                return mContext.getString(R.string.convertedData);
            default:
                return null;
        }
    }

    @Override
    public android.support.v4.app.Fragment getItem(int position) {
        if (position == 0) {
            return new FragmentHome();
        } else if (position == 1) {
            return new FragmentConverted();
        }else {
            return new FragmentHome();
        }
    }
}
