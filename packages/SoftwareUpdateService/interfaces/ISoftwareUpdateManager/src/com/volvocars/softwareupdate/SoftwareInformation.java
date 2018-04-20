/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import java.util.ArrayList;

import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.DownloadInfo;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwareInformation implements Parcelable {
    public enum SoftwareState {
        AVAILABLE, COMMISSIONED, DOWNLOAD_PENDING, DOWNLOADING_META, DOWNLOADING, DOWNLOADED, INSTALL_PENDING, INSTALLING, INSTALLED, UNDEFINED
    };

    public SoftwareState softwareState = SoftwareState.UNDEFINED;

    public SoftwareAssignment softwareAssignment = new SoftwareAssignment();
    public DownloadInfo downloadInfo = new DownloadInfo();

    public static final Creator<SoftwareInformation> CREATOR = new Creator<SoftwareInformation>() {
        @Override
        public SoftwareInformation createFromParcel(Parcel in) {
            return new SoftwareInformation(in);
        }

        @Override
        public SoftwareInformation[] newArray(int size) {
            return new SoftwareInformation[size];
        }
    };

    public SoftwareInformation() {
    }

    private SoftwareInformation(Parcel in) {
        readFromParcel(in);
    }

    public SoftwareInformation(SoftwareAssignment assignment) {
        this.softwareAssignment = assignment;
    }

    public void updateSoftwareAssignment(SoftwareAssignment assignment) {
        this.softwareAssignment = assignment;
    }

    public void AddDownloadInfo(DownloadInfo downloadInfo) {
        this.downloadInfo = downloadInfo;
        if(!this.downloadInfo.resourceUris.isEmpty()) { //TODO: remove and replace with installNotification
            this.softwareState = SoftwareState.DOWNLOADING;
        } else {
            if (!this.downloadInfo.downloadedResources.isEmpty()) {
                this.softwareState = SoftwareState.DOWNLOADED;
            }
        }
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(softwareState.toString());
        dest.writeTypedObject(softwareAssignment, flags);
        dest.writeTypedObject(downloadInfo, flags);
    }

    @Override
    public String toString() {
        String string = softwareAssignment.toString() + downloadInfo.toString();
        return string;
    }

    public void readFromParcel(Parcel in) {
        softwareState = stringToSoftwareState(in.readString());
        softwareAssignment = in.readTypedObject(SoftwareAssignment.CREATOR);
        downloadInfo = in.readTypedObject(DownloadInfo.CREATOR);
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

    public SoftwareState stringToSoftwareState(String str) {
        if (null != str) {
            switch(str) {
                case "AVAILABLE": return SoftwareState.AVAILABLE;
                case "COMMISSIONED": return SoftwareState.COMMISSIONED;
                case "DOWNLOAD_PENDING": return SoftwareState.DOWNLOAD_PENDING;
                case "DOWNLOADING_META": return SoftwareState.DOWNLOADING_META;
                case "DOWNLOADING": return SoftwareState.DOWNLOADING;
                case "DOWNLOADED": return SoftwareState.DOWNLOADED;
                case "INSTALL_PENDING": return SoftwareState.INSTALL_PENDING;
                case "INSTALLING": return SoftwareState.INSTALL_PENDING;
                case "UNDEFINED": return SoftwareState.UNDEFINED;
                default: return SoftwareState.UNDEFINED;
            }
        }

        return SoftwareState.UNDEFINED;
    }
}
