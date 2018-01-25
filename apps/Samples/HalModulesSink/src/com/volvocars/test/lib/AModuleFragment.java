/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.test.lib;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.concurrent.CompletableFuture;

public abstract class AModuleFragment extends Fragment {
    public static final String TAG = AModuleFragment.class.getSimpleName();
    private static AModuleFragment fragment;
    private static FragmentManager fragmentManager;
    private static Activity activity;

    public static AModuleFragment newInstance(FragmentManager fragmentM, AModuleFragment self,
            Activity activityTmp) {
        fragmentManager = fragmentM;
        fragment = self;
        activity = activityTmp;
        return fragment;
    }

    @Override
    public abstract View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState);

    @Override
    public abstract void onViewCreated(View view, Bundle savedInstanceState);

    public void runBackground(Runnable run) {
        CompletableFuture.runAsync(run);
    }

    public void runBackgroundAndUpdate(Runnable run) {
        CompletableFuture.runAsync(run)
                .thenRun(() -> {
                    Log.d(TAG, "Started runBackgroundAndUpdate");
                    updateFragment();
                });
    }

    public void updateFragment() {
        fragmentManager.beginTransaction().replace(fragment.getId(), fragment).commit();
    }

    public void updateUI(Runnable run) {
        activity.runOnUiThread(run);
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        //Save the fragment state handling
    }
}
