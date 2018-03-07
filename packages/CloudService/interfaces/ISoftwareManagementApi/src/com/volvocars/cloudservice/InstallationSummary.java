/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import java.util.List;
import java.util.ArrayList;

import android.os.Parcel;
import android.os.Parcelable;

public class InstallationSummary implements Parcelable {

    public String softwareId = "";
    public String timestamp = "";
    public int repeatResets = 0;
    public long totalInstallationTime = 0;

    List<Ecu> ecus = new ArrayList();

    public static final Creator<InstallationSummary> CREATOR = new Creator<InstallationSummary>() {
        @Override
        public InstallationSummary createFromParcel(Parcel in) {
            return new InstallationSummary(in);
        }

        @Override
        public InstallationSummary[] newArray(int size) {
            return new InstallationSummary[size];
        }
    };

    public InstallationSummary() {
    }

    private InstallationSummary(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(softwareId);
        dest.writeString(timestamp);
        dest.writeInt(repeatResets);
        dest.writeLong(totalInstallationTime);
        dest.writeTypedList(ecus);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        softwareId = in.readString();
        timestamp = in.readString();
        repeatResets = in.readInt();
        totalInstallationTime = in.readLong();

        in.readTypedList(ecus, Ecu.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}