/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.os.Parcel;
import android.os.Parcelable;

public enum UserSwitchResult implements Parcelable {
    SUCCESS,
    ERROR,
    UNKNOWN;

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(name());
    }

    public static final Creator<UserSwitchResult> CREATOR = new Creator<UserSwitchResult>() {
        @Override
        public UserSwitchResult createFromParcel(final Parcel source) {
            return UserSwitchResult.valueOf(source.readString());
        }

        @Override
        public UserSwitchResult[] newArray(final int size) {
            return new UserSwitchResult[size];
        }
    };
}