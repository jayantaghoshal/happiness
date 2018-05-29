/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.downloadcenter;

import android.app.Application;
import android.arch.lifecycle.AndroidViewModel;
import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.support.annotation.NonNull;

import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.SoftwareUpdateManager;
import com.volvocars.softwareupdate.SoftwareUpdateManagerCallback;

import java.util.List;

public class MainViewModel extends AndroidViewModel {

    private final MutableLiveData<Boolean> mIsConnected = new MutableLiveData<>();
    private final MutableLiveData<List<SoftwareAssignment>> mSoftwareAssignments = new MutableLiveData<>();

    private final SoftwareUpdateManager mSoftwareUpdateManager;

    public MainViewModel(@NonNull Application application) {
        super(application);

//        softwareUpdateManager.setCallback();
//        softwareUpdateManager.connect();

        mIsConnected.postValue(false);

        mSoftwareUpdateManager = new SoftwareUpdateManager(application, new SoftwareUpdateManagerCallback() {
            @Override
            public void onServiceConnected() {
                mIsConnected.postValue(true);
            }

            @Override
            public void onServiceDisconnected() {
                mIsConnected.postValue(false);
            }
        });
    }

    public LiveData<Boolean> getIsConnected() {
        return mIsConnected;
    }


    @Override
    protected void onCleared() {
        super.onCleared();
        mSoftwareUpdateManager.disconnect();
    }
}
