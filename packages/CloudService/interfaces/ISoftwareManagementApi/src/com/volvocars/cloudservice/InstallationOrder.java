/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class InstallationOrder implements Parcelable {
    public String uuid = "";
    public String status = "";
    public String createdBy = "";
    public String created = "";
    public String downloadsUri = "";
    public String installNotificationsUri = "";
    public String installationReportUri = "";
    public SoftwareAssignment software = null;

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
        dest.writeString(uuid);
        dest.writeString(status);
        dest.writeString(createdBy);
        dest.writeString(created);
        dest.writeString(downloadsUri);
        dest.writeString(installNotificationsUri);
        dest.writeString(installationReportUri);
        dest.writeTypedObject(software, 0);
    }

    @Override
    public String toString() {
        String string = uuid + "\n" + status + "\n" + createdBy + "\n" + created + "\n" + downloadsUri + "\n"
                + installNotificationsUri + "\n" + installationReportUri + "\n" + software.uuid + "\n" + software.name
                + "\n" + software.description;
        return string;
    }

    public void readFromParcel(Parcel in) {
        uuid = in.readString();
        status = in.readString();
        createdBy = in.readString();
        created = in.readString();
        downloadsUri = in.readString();
        installNotificationsUri = in.readString();
        installationReportUri = in.readString();
        software = in.readTypedObject(SoftwareAssignment.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
