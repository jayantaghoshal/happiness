/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import java.util.List;
import java.util.ArrayList;

import android.os.Parcel;
import android.os.Parcelable;

public class DownloadSummary implements Parcelable {

    public String timestamp = "";
    public long totalDownloadTime = 0;
    public long effectiveDownloadTime = 0;

    List<DataFile> dataFiles = new ArrayList();

    public static final Creator<DownloadSummary> CREATOR = new Creator<DownloadSummary>() {
        @Override
        public DownloadSummary createFromParcel(Parcel in) {
            return new DownloadSummary(in);
        }

        @Override
        public DownloadSummary[] newArray(int size) {
            return new DownloadSummary[size];
        }
    };

    public DownloadSummary() {
    }

    private DownloadSummary(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(timestamp);
        dest.writeLong(totalDownloadTime);
        dest.writeLong(effectiveDownloadTime);
        dest.writeTypedList(dataFiles);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        timestamp = in.readString();
        totalDownloadTime = in.readLong();
        effectiveDownloadTime = in.readLong();

        in.readTypedList(dataFiles, DataFile.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}