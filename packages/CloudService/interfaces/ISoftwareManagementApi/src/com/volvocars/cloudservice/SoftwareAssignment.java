/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwareAssignment implements Parcelable {

    public enum Status {
        UNKNOWN, COMMISSIONABLE, NON_COMMISSIONABLE, COMMISSIONED
    }

    public enum Type {
        UNKNOWN, SYSTEM, ACCESSORY
    }

    public enum DeliverableType {
        UNKNOWN, UPDATE
    }

    public enum InstallationType {
        UNKNOWN, BOOT, NORMAL
    }

    public static class LongDescription implements Parcelable {
        public String textBlock = "";
        public String listElement = "";
        public String lineFeed = "";

        public LongDescription(Parcel in) {
            readFromParcel(in);
        }

        public LongDescription() {
        }

        public static final Creator<LongDescription> CREATOR = new Creator<LongDescription>() {
            @Override
            public LongDescription createFromParcel(Parcel in) {
                return new LongDescription(in);
            }

            @Override
            public LongDescription[] newArray(int size) {
                return new LongDescription[size];
            }
        };

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeString(textBlock);
            dest.writeString(listElement);
            dest.writeString(lineFeed);
        }

        public void readFromParcel(Parcel in) {
            textBlock = in.readString();
            listElement = in.readString();
            lineFeed = in.readString();
        }

        @Override
        public int describeContents() {
            // As long there are no children, this function is quite useless?
            return 0;
        }
    }

    //Unique id of the software
    public String id = "";
    //The name of the software
    public String name = "";
    //Graphical symbol representing the actual software.
    public byte[] icon = {};
    //A short description of the software
    public String shortDescription = "";
    //An extended description of the software. Made up of combination of textblock, listelement and linefeed.
    public LongDescription longDescription = new LongDescription();
    //The download size of this software in MB
    public double size = 0.0;
    //Software product id.
    public String softwareProductId = "";
    //Software product version.
    public String softwareProductVersion = "";
    //Software sub status can be either: COMMISSIONABLE, NON-COMMISSIONABLE, COMMISSIONED
    public Status status = Status.UNKNOWN;
    //Software type can be either: SYSTEM, ACCESSORY
    public Type type = Type.UNKNOWN;
    //Deliverable type can be either: UPDATE
    public DeliverableType deliverableType = DeliverableType.UNKNOWN;
    //Installation type can be either: BOOT, NORMAL
    public InstallationType installationType = InstallationType.UNKNOWN;
    //Estimated installation time for the software. Mandatory for BOOT.
    public int estimatedInstallationTime = -1;
    //URI for commissioning the software. Only shown if software/status is COMMISSIONABLE or COMMISSIONED.
    public String commissionUri = "";
    //Installation order element with commissioned software.
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
        dest.writeString(id);
        dest.writeString(name);
        dest.writeInt(icon.length);
        for (byte b : icon) {
            dest.writeByte(b);
        }
        dest.writeString(shortDescription);
        dest.writeTypedObject(longDescription, 0);
        dest.writeDouble(size);
        dest.writeString(softwareProductId);
        dest.writeString(softwareProductVersion);
        dest.writeString(status.toString());
        dest.writeString(type.toString());
        dest.writeString(deliverableType.toString());
        dest.writeString(installationType.toString());
        dest.writeInt(estimatedInstallationTime);
        dest.writeString(commissionUri);
        dest.writeTypedObject(installationOrder, 0);
    }

    @Override
    public String toString() {
        String string = id + "\n" + name + "\n" + shortDescription + "\n" + status + "\n" + installationOrder.status;
        return string;
    }

    public void readFromParcel(Parcel in) {
        id = in.readString();
        name = in.readString();
        int image_size = in.readInt();
        icon = new byte[image_size];

        for (int i = 0; i < image_size; i++) {
            icon[i] = in.readByte();
        }
        shortDescription = in.readString();
        longDescription = in.readTypedObject(LongDescription.CREATOR);
        size = in.readDouble();
        softwareProductId = in.readString();
        softwareProductVersion = in.readString();
        status = stringToStatus(in.readString());
        type = stringToType(in.readString());
        deliverableType = stringToDeliverableType(in.readString());
        installationType = stringToInstallationType(in.readString());
        estimatedInstallationTime = in.readInt();
        commissionUri = in.readString();
        installationOrder = in.readTypedObject(InstallationOrder.CREATOR);
    }

    public static Status stringToStatus(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return Status.UNKNOWN;
            case "COMMISSIONABLE":
                return Status.COMMISSIONABLE;
            case "NON_COMMISSIONABLE":
                return Status.NON_COMMISSIONABLE;
            case "COMMISSIONED":
                return Status.COMMISSIONED;
            default:
                return Status.UNKNOWN;
            }
        } else {
            return Status.UNKNOWN;
        }
    }

    public static Type stringToType(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return Type.UNKNOWN;
            case "SYSTEM":
                return Type.SYSTEM;
            case "ACCESSORY":
                return Type.ACCESSORY;
            default:
                return Type.UNKNOWN;
            }
        } else {
            return Type.UNKNOWN;
        }
    }

    public static DeliverableType stringToDeliverableType(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return DeliverableType.UNKNOWN;
            case "UPDATE":
                return DeliverableType.UPDATE;
            default:
                return DeliverableType.UNKNOWN;
            }
        } else {
            return DeliverableType.UNKNOWN;
        }

    }

    public static InstallationType stringToInstallationType(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return InstallationType.UNKNOWN;
            case "BOOT":
                return InstallationType.BOOT;
            case "NORMAL":
                return InstallationType.NORMAL;
            default:
                return InstallationType.UNKNOWN;
            }
        } else {
            return InstallationType.UNKNOWN;
        }
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
