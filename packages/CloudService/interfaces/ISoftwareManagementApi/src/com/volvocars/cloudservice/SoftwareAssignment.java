/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwareAssignment implements Parcelable {

    private enum Status {
        UNKNOWN, COMMISSIONABLE, NON_COMMISSIONABLE, COMMISSIONED
    }

    public String uuid = "dummy";
    public String name = "dummy";
    public String description = "dummy";
    public byte[] image = {};
    public String softwareProductId = "";
    public String softwareProductVersion = "";
    public Status status = Status.UNKNOWN;
    public InstallationOrder installationOrder = new InstallationOrder();

    public static final Creator<SoftwareAssignment> CREATOR = new Creator<SoftwareAssignment>() {
        @Override
        public SoftwareAssignment createFromParcel(Parcel in) {
            return new SoftwareAssignment(in);
        }

        @Override
        public SoftwareAssignment[] newArray(int size) {
            return new SoftwareAssignment[size];
        }
    };

    public SoftwareAssignment() {
    }

    private SoftwareAssignment(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(uuid);
        dest.writeString(name);
        dest.writeString(description);
        dest.writeString(softwareProductId);
        dest.writeString(softwareProductVersion);
        dest.writeString(status.toString());
        dest.writeTypedObject(installationOrder, 0);
        dest.writeInt(image.length);
        for (byte b : image) {
            dest.writeByte(b);
        }
    }

    @Override
    public String toString() {
        String string = uuid + "\n" + name + "\n" + description + "\n";
        return string;
    }

    public void readFromParcel(Parcel in) {
        uuid = in.readString();
        name = in.readString();
        description = in.readString();
        softwareProductId = in.readString();
        softwareProductVersion = in.readString();
        status = Status.valueOf(in.readString());
        installationOrder = in.readTypedObject(InstallationOrder.CREATOR);

        int image_size = in.readInt();
        image = new byte[image_size];

        for (int i = 0; i < image_size; i++) {
            image[i] = in.readByte();
        }
    }

    public static Status stringToStatus(String str) {
        switch (str) {
            case "UNKNOWN": return Status.UNKNOWN;
            case "COMMISSIONABLE": return Status.COMMISSIONABLE;
            case "NON_COMMISSIONABLE": return Status.NON_COMMISSIONABLE;
            case "COMMISSIONED": return Status.COMMISSIONED;
            default: return Status.UNKNOWN;
        }
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
