/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance;

import android.app.Application;
import android.arch.lifecycle.ViewModel;
import android.arch.lifecycle.ViewModelProvider;
import android.arch.lifecycle.ViewModelProviders;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.util.Log;

import com.volvocars.vendorextension.VendorExtensionClient;

public class AssistanceViewModelFactory {

    private final Application mApplication;

    public AssistanceViewModelFactory(Application application) {
        mApplication = application;
    }

    public AssistanceViewModel get(Fragment fragment) {
        Factory factory = new Factory();
        return ViewModelProviders.of(fragment, factory).get(AssistanceViewModel.class);
    }

    public AssistanceViewModel get(FragmentActivity fragmentActivity) {
        Factory factory = new Factory();
        return ViewModelProviders.of(fragmentActivity, factory).get(AssistanceViewModel.class);
    }

    class Factory implements ViewModelProvider.Factory {
        Factory() {
        }

        @Override
        public <T extends ViewModel> T create(Class<T> modelClass) {
            if (modelClass == AssistanceViewModel.class) {
                return (T) new AssistanceViewModel(VendorExtensionClient.getVolvoExtension(mApplication));
            }
            throw new RuntimeException("AssistanceViewModelFactory doesn't know how to create: "
                    + modelClass.toString());
        }
    }
}