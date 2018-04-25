/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.util.AttributeSet;
import android.view.MotionEvent;

/**
 * CustomViewpager class to handle swipe
 */
public class CustomViewPager extends ViewPager{

    private boolean mIsPagingEnabled;

    public CustomViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean onTouchEvent(MotionEvent motionevent) {
        return this.mIsPagingEnabled && super.onTouchEvent(motionevent);
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent motionevent) {
        return this.mIsPagingEnabled && super.onInterceptTouchEvent(motionevent);
    }

    public void setPagingEnabled(Boolean isEnabled){
        this.mIsPagingEnabled = isEnabled;
    }
}