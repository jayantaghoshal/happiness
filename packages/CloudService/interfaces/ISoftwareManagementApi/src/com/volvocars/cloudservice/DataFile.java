/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class DataFile implements Parcelable {

    public enum TargetStorageId {
        UNKNOWN,
        LOCAL_STORAGE_NORMAL,
        LOCAL_STORAGE_BOOT
    }

    public enum Status {
        UNKNOWN,
        STORED,
        DOWNLOADED,
        FAILED,
        NOT_STARTED,
        VERIFIED
    }

    public String identifier = "";
    public TargetStorageId targetStorageId = TargetStorageId.UNKNOWN;
    public Status status = Status.UNKNOWN;

    public static final Creator<DataFile> CREATOR = new Creator<DataFile>() {
        @Override
        public DataFile createFromParcel(Parcel in) {
            return new DataFile(in);
        }

        @Override
        public DataFile[] newArray(int size) {
            return new DataFile[size];
        }
    };

    public DataFile() {
    }

    private DataFile(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(identifier);
        dest.writeString(targetStorageId.toString());
        dest.writeString(status.toString());
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        identifier = in.readString();
        targetStorageId = TargetStorageId.valueOf(in.readString());
        status = Status.valueOf(in.readString());
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}