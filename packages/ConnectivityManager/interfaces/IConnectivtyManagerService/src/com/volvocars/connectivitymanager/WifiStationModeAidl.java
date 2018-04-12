/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager;

import android.os.Parcel;
import android.os.Parcelable;

public class WifiStationModeAidl implements Parcelable {

    public enum Mode {
        UNKNOWN,
        AP_MODE,
        STATION_MODE
    }
    public Mode mode = Mode.UNKNOWN;

    public static final Creator<WifiStationModeAidl> CREATOR = new Creator<WifiStationModeAidl>() {
        @Override
        public WifiStationModeAidl createFromParcel(Parcel in) {
            return new WifiStationModeAidl(in);
        }

        @Override
        public WifiStationModeAidl[] newArray(int size) {
            return new WifiStationModeAidl[size];
        }
    };

    public WifiStationModeAidl() {
    }

    private WifiStationModeAidl(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mode.toString());
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        mode = Mode.valueOf(in.readString());
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
