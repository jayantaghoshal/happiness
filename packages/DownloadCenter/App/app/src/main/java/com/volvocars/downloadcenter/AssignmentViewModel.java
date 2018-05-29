/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.downloadcenter;

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;

import com.volvocars.cloudservice.SoftwareAssignment;

public class AssignmentViewModel {

    private final MutableLiveData<String> mTitle = new MutableLiveData<>();

    public AssignmentViewModel(SoftwareAssignment softwareAssignment) {
        bind(softwareAssignment);
    }

    public void bind(SoftwareAssignment softwareAssignment) {
         mTitle.setValue(softwareAssignment.name);
    }
}
