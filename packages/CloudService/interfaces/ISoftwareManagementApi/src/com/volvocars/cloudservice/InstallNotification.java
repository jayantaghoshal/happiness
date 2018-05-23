/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class InstallNotification implements Parcelable {
    //Unique id of the software
    public String softwareId = "";
    //Unique id of the installation order
    public String installationOrderId = "";
    public Notification notification = new Notification();

    //Install notification uri, this is retrieved from the software assignment
    public String uri = "";

    public static final Creator<InstallNotification> CREATOR = new Creator<InstallNotification>() {
        @Override
        public InstallNotification createFromParcel(Parcel in) {
            return new InstallNotification(in);
        }

        @Override
        public InstallNotification[] newArray(int size) {
            return new InstallNotification[size];
        }
    };

    public InstallNotification() {
    }

    private InstallNotification(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(softwareId);
        dest.writeString(installationOrderId);
        dest.writeTypedObject(notification, 0);
        dest.writeString(uri);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        softwareId = in.readString();
        installationOrderId = in.readString();
        notification = in.readTypedObject(Notification.CREATOR);
        uri = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

}