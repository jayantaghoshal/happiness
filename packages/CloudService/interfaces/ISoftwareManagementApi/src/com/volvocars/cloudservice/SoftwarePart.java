/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwarePart implements Parcelable {

    public enum Status {
        UNKNOWN,
        UN_INSTALLED,
        INSTALLED,
        FAILED_CRITICAL,
        FAILED,
        NOT_STARTED
    }

    public String identifier = "";
    public int retries = 0;
    public long measuredInstallationTime = 0;
    public Status status = Status.UNKNOWN;

    public static final Creator<SoftwarePart> CREATOR = new Creator<SoftwarePart>() {
        @Override
        public SoftwarePart createFromParcel(Parcel in) {
            return new SoftwarePart(in);
        }

        @Override
        public SoftwarePart[] newArray(int size) {
            return new SoftwarePart[size];
        }
    };

    public SoftwarePart() {
    }

    private SoftwarePart(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(identifier);
        dest.writeInt(retries);
        dest.writeLong(measuredInstallationTime);
        dest.writeString(status.toString());
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        identifier = in.readString();
        retries = in.readInt();
        measuredInstallationTime = in.readLong();
        status = Status.valueOf(in.readString());

    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}