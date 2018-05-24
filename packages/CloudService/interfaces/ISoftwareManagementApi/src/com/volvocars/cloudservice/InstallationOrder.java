/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;
public class InstallationOrder implements Parcelable {
    //Unique id of the installation order.
    public String id = "";
    //URI for the Downloads resource. Fetch the download resource before starting the download and installation process.
    public String downloadsUri = "";
    //URI to POST or GET install notifications.
    //Only visble if installation order status is READY
    public String installNotificationsUri = "";
    //URI to POST InstallationReport.
    //Only visible if installation order status is READY
    public String installationReportUri = "";
    public static final Creator<InstallationOrder> CREATOR = new Creator<InstallationOrder>() {
        @Override
        public InstallationOrder createFromParcel(Parcel in) {
            return new InstallationOrder(in);
        }

        @Override
        public InstallationOrder[] newArray(int size) {
            return new InstallationOrder[size];
        }
    };

    public InstallationOrder() {
    }

    private InstallationOrder(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(id);
        dest.writeString(downloadsUri);
        dest.writeString(installNotificationsUri);
        dest.writeString(installationReportUri);
    }

    @Override
    public String toString() {
        String string = id + "\n" + downloadsUri + "\n"
                + installNotificationsUri + "\n" + installationReportUri;
        return string;
    }

    public void readFromParcel(Parcel in) {
        id = in.readString();
        downloadsUri = in.readString();
        installNotificationsUri = in.readString();
        installationReportUri = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
