/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.Parcel;
import android.os.Parcelable;

public enum InstallOption implements Parcelable {
    CANCEL, INSTALL, POSTPONE;

    public static final Creator<InstallOption> CREATOR = new Creator<InstallOption>() {
        @Override
        public InstallOption createFromParcel(Parcel in) {
            return InstallOption.valueOf(in.readString());
        }

        @Override
        public InstallOption[] newArray(int size) {
            return new InstallOption[size];
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
