/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class InstallationReport implements Parcelable {

    public enum ReportReason {
        UNKNOWN,
        OK,
        FAILED,
        FAILED_CRITICAL,
        CANCELLED
    }

    public String installationOrderId = "";
    public String timestamp = "";
    public ReportReason reportReason = ReportReason.UNKNOWN;

    public DownloadSummary downloadSummary = new DownloadSummary();
    public InstallationSummary installationSummary = new InstallationSummary();

     //Installation report uri, this is retrieved from the software assignment
     public String uri = "";

    public static final Creator<InstallationReport> CREATOR = new Creator<InstallationReport>() {
        @Override
        public InstallationReport createFromParcel(Parcel in) {
            return new InstallationReport(in);
        }

        @Override
        public InstallationReport[] newArray(int size) {
            return new InstallationReport[size];
        }
    };

    public InstallationReport() {
    }

    private InstallationReport(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(installationOrderId);
        dest.writeString(timestamp);
        dest.writeString(reportReason.toString());
        dest.writeTypedObject(downloadSummary, 0);
        dest.writeTypedObject(installationSummary, 0);
        dest.writeString(uri);
    }

    @Override
    public String toString() {
        String string = "";
        return string;
    }

    public void readFromParcel(Parcel in) {
        installationOrderId = in.readString();
        timestamp = in.readString();
        reportReason = ReportReason.valueOf(in.readString());

        downloadSummary = in.readTypedObject(DownloadSummary.CREATOR);
        installationSummary = in.readTypedObject(InstallationSummary.CREATOR);
        uri = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}