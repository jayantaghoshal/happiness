/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager;

import android.os.Parcel;
import android.os.Parcelable;

public class WifiStationMode implements Parcelable {

    public enum Mode {
        UNKNOWN,
        AP_MODE,
        STATION_MODE
    }
    public Mode mode = Mode.UNKNOWN;

    public static final Creator<WifiStationMode> CREATOR = new Creator<WifiStationMode>() {
        @Override
        public WifiStationMode createFromParcel(Parcel in) {
            return new WifiStationMode(in);
        }

        @Override
        public WifiStationMode[] newArray(int size) {
            return new WifiStationMode[size];
        }
    };

    public WifiStationMode() {
    }

    private WifiStationMode(Parcel in) {
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