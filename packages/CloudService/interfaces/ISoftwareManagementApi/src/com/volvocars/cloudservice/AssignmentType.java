/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public enum AssignmentType implements Parcelable {
    UPDATE, ACCESSORY;

    public static final Creator<AssignmentType> CREATOR = new Creator<AssignmentType>() {
        @Override
        public AssignmentType createFromParcel(Parcel in) {
            return AssignmentType.valueOf(in.readString());
        }

        @Override
        public AssignmentType[] newArray(int size) {
            return new AssignmentType[size];
        }
    };

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(name());
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}