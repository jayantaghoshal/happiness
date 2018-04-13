/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;
public class InstallationOrder implements Parcelable {

    public enum Status {
        UNKNOWN, PREPARING, READY
    }

    //Unique id of the installation order.
    public String id = "";
    //Status can be either: PREPARING (Will be implemented/used later), READY
    public Status status = Status.UNKNOWN;
    //Id of the client that have done commission for this software, e.g. VIN for vehicle or volvoid of the user.
    public String createdBy = "";
    //Date and time of when the software was commissioned. Created when the backend service received the POST.
    public String created = "";
    //URI for the Downloads resource. Fetch the download resource before starting the download and installation process.
    //Only visible if installation order status is READY
    public String downloadsUri = "";
    //URI to POST or GET install notifications.
    //Only visble if installation order status is READY
    public String installNotificationsUri = "";
    //URI to POST InstallationReport.
    //Only visible if installation order status is READY
    public String installationReportUri = "";
    private static final String LOG_TAG = "InstallationOrder";

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
        dest.writeString(status.toString());
        dest.writeString(createdBy);
        dest.writeString(created);
        dest.writeString(downloadsUri);
        dest.writeString(installNotificationsUri);
        dest.writeString(installationReportUri);
    }

    @Override
    public String toString() {
        String string = id + "\n" + status + "\n" + createdBy + "\n" + created + "\n" + downloadsUri + "\n"
                + installNotificationsUri + "\n" + installationReportUri;
        return string;
    }

    public void readFromParcel(Parcel in) {
        id = in.readString();
        status = stringToStatus(in.readString());
        createdBy = in.readString();
        created = in.readString();
        downloadsUri = in.readString();
        installNotificationsUri = in.readString();
        installationReportUri = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

    public static Status stringToStatus(String str) {
        if(null != str) {
            switch(str) {
                case "UNKNOWN": return Status.UNKNOWN;
                case "PREPARING": return Status.PREPARING;
                case "READY": return Status.READY;
                default: return Status.UNKNOWN;
            }
        } else {
            return Status.UNKNOWN;
        }
    }
}
