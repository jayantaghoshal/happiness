/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class Status implements Parcelable {
    //Status code on software product level
    public StatusCode statusCode = StatusCode.UNKNOWN;
    //Extended status information on software product level
    public SubStatusCode subStatusCode = SubStatusCode.UNKNOWN;
    //Extended reason information on software product level
    public SubStatusReason subStatusReason = SubStatusReason.UNKNOWN;

    public enum StatusCode {
        UNKNOWN, OK, FAILED, IN_PROGRESS, CANCELLED
    }

    public enum SubStatusCode {
        UNKNOWN, ASSIGNMENT_COMMISSIONED, DOWNLOAD_STARTED, DOWNLOAD_COMPLETED,
        INSTALLATION_IN_PROGRESS, INSTALLATION_STARTED, UNINSTALLATION_STARTED,
        INSTALLATION_DEFERRED, VALIDATION_OK, DOWNLOAD_OK, WAIT_FOR_RESTART,
        ASSIGNMENT_REJECTED, DOWNLOAD_ABORTED, INSTALLATION_ABORTED, DOWNLOAD_FAILED,
        INSTALLATION_FAILED, INSTALLATION_FAILED_CRITICAL, NO_DIAGNOSTIC_RESPONSE,
        VALIDATION_FAILED, INSTALLATION_COMPLETED, UNINSTALLATION_COMPLETED, UNINSTALLATION_FAILED
    }

    public enum SubStatusReason {
        UNKNOWN, AUTOMATIC_UPDATE, USER, REMOTE_UNINSTALL, ON_BOARD_UNINSTALL, CLIENT,
        WAIT_FOR_RESTART, BOOT, NORMAL, ENERGY_LOW, SYSTEM, TIMEOUT, USAGEMODE_NOT_ALLOWED,
        USER_POSTPONE, WITHDRAWN, AUTHENTICATION_INVALID, CLIENT_ERROR, CONFIGURATION_MISMATCH,
        DOWNLOAD_ERROR, INVALID_DATA, REQUEST_NOT_ALLOWED, OTHER_FAILURE
    }

    public static final Creator<Status> CREATOR = new Creator<Status>() {
        @Override
        public Status createFromParcel(Parcel in) {
            return new Status(in);
        }

        @Override
        public Status[] newArray(int size) {
            return new Status[size];
        }
    };

    public Status() {
    }

    private Status(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(statusCode.toString());
        dest.writeString(subStatusCode.toString());
        dest.writeString(subStatusReason.toString());
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        statusCode = StatusCode.valueOf(in.readString());
        subStatusCode = SubStatusCode.valueOf(in.readString());
        subStatusReason = SubStatusReason.valueOf(in.readString());
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}