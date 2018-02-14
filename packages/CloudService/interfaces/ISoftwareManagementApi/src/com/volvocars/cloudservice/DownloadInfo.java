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
    public String install_notification_uri = "";
    public String installation_order_id = "";
    public ArrayList<String> downloads = new ArrayList();

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
        dest.writeString(install_notification_uri);
        dest.writeString(installation_order_id);
        dest.writeInt(downloads.size());
        for (String d : downloads) {
            dest.writeString(d);
        }
    }

    @Override
    public String toString() {
        String string = uuid + "\n" + install_notification_uri + "\n" + installation_order_id;
        return string;
    }

    public void readFromParcel(Parcel in) {
        uuid = in.readString();
        install_notification_uri = in.readString();
        installation_order_id = in.readString();
        int size = in.readInt();

        for (int i = 0; i  < size; i++) {
            downloads.add(in.readString());
        }
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
