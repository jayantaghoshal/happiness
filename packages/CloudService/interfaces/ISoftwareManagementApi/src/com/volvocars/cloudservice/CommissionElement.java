/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class CommissionElement implements Parcelable {

    public enum Action {
        UNKNOWN, ORDER_SOFTWARE_INSTALLATION, REQUEST_INSTALLATION_CANCELLATION;
    }

    public enum Reason {
        UNKNOWN, AUTOMATIC_UPDATE, USER
    }

    //Unique id of the software.
    public String id = "";
    //Unique id of the client that performs the commission, eg. "VIN" for a vehicle or "volvoid" for a user.
    public String clientId = "";
    public Action action = Action.UNKNOWN;
    public Reason reason = Reason.UNKNOWN;

    //uri resource URI for commissioning the software (set by corresponding software assignment)
    public String commissionUri = "";

    public static final Creator<CommissionElement> CREATOR = new Creator<CommissionElement>() {
        @Override
        public CommissionElement createFromParcel(Parcel in) {
            return new CommissionElement(in);
        }

        @Override
        public CommissionElement[] newArray(int size) {
            return new CommissionElement[size];
        }
    };

    public CommissionElement() {
    }

    private CommissionElement(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(id);
        dest.writeString(clientId);
        dest.writeString(action.toString());
        dest.writeString(reason.toString());
        dest.writeString(commissionUri);
    }

    @Override
    public String toString() {
        String string = id + "\n" + clientId + "\n" + action.toString() + "\n" + reason.toString();
        return string;
    }

    public void readFromParcel(Parcel in) {
        id = in.readString();
        clientId = in.readString();
        action = stringToAction(in.readString());
        reason = stringToReason(in.readString());
        commissionUri = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

    public static Action stringToAction(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return Action.UNKNOWN;
            case "ORDER_SOFTWARE_INSTALLATION":
                return Action.ORDER_SOFTWARE_INSTALLATION;
            case "REQUEST_INSTALLATION_CANCELLATION":
                return Action.REQUEST_INSTALLATION_CANCELLATION;
            default:
                return Action.UNKNOWN;
            }
        } else {
            return Action.UNKNOWN;
        }
    }

    public static Reason stringToReason(String str) {
        if (null != str) {
            switch (str) {
            case "UNKNOWN":
                return Reason.UNKNOWN;
            case "AUTOMATIC_UPDATE":
                return Reason.AUTOMATIC_UPDATE;
            case "USER":
                return Reason.USER;
            default:
                return Reason.UNKNOWN;
            }
        } else {
            return Reason.UNKNOWN;
        }
    }
}
