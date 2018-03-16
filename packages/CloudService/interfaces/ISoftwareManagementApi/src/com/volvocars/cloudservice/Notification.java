/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class Notification implements Parcelable {
    //Unique id of the installation notification
    public String id = "";
    //Date and time of when the install notification was created
    //(when the back-end service received the POST)
    public String created = "";
    //Client's creation date of install notificaton
    public String timestamp = "";

    public Status status  = new Status();


    public static final Creator<Notification> CREATOR = new Creator<Notification>() {
        @Override
        public Notification createFromParcel(Parcel in) {
            return new Notification(in);
        }

        @Override
        public Notification[] newArray(int size) {
            return new Notification[size];
        }
    };

    public Notification() {
    }

    private Notification(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(id);
        dest.writeString(created);
        dest.writeString(timestamp);
        dest.writeTypedObject(status, 0);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        id = in.readString();
        created = in.readString();
        timestamp = in.readString();

        status = in.readTypedObject(Status.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}