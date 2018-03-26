/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.ArrayList;
import java.util.List;

public class Ecu implements Parcelable {

    public enum Status {
        UNKNOWN,
        VERIFIED,
        INSTALLED,
        FAILED_CRITICAL,
        FAILED,
        NOT_STARTED
    }

    public int address = 0;
    public int retries = 0;
    public Status status = Status.UNKNOWN;

    public List<SoftwarePart> softwareParts = new ArrayList();

    public static final Creator<Ecu> CREATOR = new Creator<Ecu>() {
        @Override
        public Ecu createFromParcel(Parcel in) {
            return new Ecu(in);
        }

        @Override
        public Ecu[] newArray(int size) {
            return new Ecu[size];
        }
    };

    public Ecu() {
    }

    private Ecu(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(address);
        dest.writeInt(retries);
        dest.writeString(status.toString());
        dest.writeTypedList(softwareParts);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        address = in.readInt();
        retries = in.readInt();
        status = Status.valueOf(in.readString());

        in.readTypedList(softwareParts, SoftwarePart.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}