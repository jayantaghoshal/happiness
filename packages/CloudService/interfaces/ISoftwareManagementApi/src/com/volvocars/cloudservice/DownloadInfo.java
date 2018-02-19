/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.ArrayList;

public class DownloadInfo implements Parcelable {
    public String uuid = "";
    public String installNotificationUri = "";
    public String installationOrderId = "";
    public ArrayList<String> resourceUris = new ArrayList();
    public ArrayList<String> downloadedResources = new ArrayList();

    public static final Creator<DownloadInfo> CREATOR = new Creator<DownloadInfo>() {
        @Override
        public DownloadInfo createFromParcel(Parcel in) {
            return new DownloadInfo(in);
        }

        @Override
        public DownloadInfo[] newArray(int size) {
            return new DownloadInfo[size];
        }
    };

    public DownloadInfo() {
    }

    private DownloadInfo(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(uuid);
        dest.writeString(installNotificationUri);
        dest.writeString(installationOrderId);
        dest.writeStringList(resourceUris);
        dest.writeStringList(downloadedResources);
    }

    @Override
    public String toString() {
        String string = uuid + "\n" + installNotificationUri + "\n" + installationOrderId;
        return string;
    }

    public void readFromParcel(Parcel in) {
        uuid = in.readString();
        installNotificationUri = in.readString();
        installationOrderId = in.readString();
        resourceUris = in.createStringArrayList();
        downloadedResources = in.createStringArrayList();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
