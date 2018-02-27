/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import java.util.ArrayList;

import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.DownloadInfo;;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwareInformation implements Parcelable {
    public enum SoftwareState {
        AVAILABLE, COMMISSIONED, DOWNLOAD_PENDING, DOWNLOADING_META, DOWNLOADING, DOWNLOADED, INSTALL_PENDING, INSTALLING, INSTALLED, UNDEFINED
    };

    public SoftwareState softwareState = SoftwareState.UNDEFINED;

    public String softwareId = "";
    public String name = "";
    public String description = "";
    //public List<Byte> image;

    public String installationId = "";
    public String installationStatus = "";

    public ArrayList<String> downloads = new ArrayList();
    public String downloadFilePath = "";

    public ArrayList<String> downloadedResources = new ArrayList();

    public static final Creator<SoftwareInformation> CREATOR =
        new Creator<SoftwareInformation>() {
            @Override public SoftwareInformation createFromParcel(Parcel in) {
                return new SoftwareInformation(in);
            }

            @Override public SoftwareInformation[] newArray(int size) {
                return new SoftwareInformation[size];
            }
        };

    public SoftwareInformation() {
    }

    private SoftwareInformation(Parcel in) {
        readFromParcel(in);
    }

    public SoftwareInformation(SoftwareAssignment assignment) {
        this.softwareState = SoftwareState.AVAILABLE;
        this.softwareId = assignment.uuid;
        this.name = assignment.name;
        this.description = assignment.description;
    }

    public SoftwareInformation(InstallationOrder installationOrder) {
        this.softwareState = SoftwareState.DOWNLOAD_PENDING;
        this.softwareId = installationOrder.software.uuid;
        this.name = installationOrder.software.name;
        this.description = installationOrder.software.description;

        this.installationId = installationOrder.uuid;
        this.installationStatus = installationOrder.status;
    }

    public void AddInstallationOrder(InstallationOrder installationOrder) {
        this.softwareState = SoftwareState.DOWNLOAD_PENDING;
        this.installationId = installationOrder.uuid;
        this.installationStatus = installationOrder.status;
    }

    public void AddDownloadInfo(DownloadInfo downloadInfo) {
        this.softwareState = SoftwareState.DOWNLOADING_META;
        this.downloads = downloadInfo.resourceUris;
        this.downloadedResources = downloadInfo.downloadedResources;

        if(!this.downloads.isEmpty()) {
            this.softwareState = SoftwareState.DOWNLOADING;
        } else {
            if (!this.downloadedResources.isEmpty()) {
                this.softwareState = SoftwareState.DOWNLOADED;
            }
        }
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(softwareState.name());
        dest.writeString(softwareId);
        dest.writeString(name);
        dest.writeString(description);

        dest.writeString(installationId);
        dest.writeString(installationStatus);

        dest.writeStringList(downloads);
        dest.writeString(downloadFilePath);

        dest.writeStringList(downloadedResources);
    }

    @Override
    public String toString() {
        String string = softwareId + "\n" + softwareState.name() + "\n" + name + "\n" + description +
                        "\nDownloads: " + downloads.toString() + "\nResources: " + downloadedResources.toString();
        return string;
    }

    public void readFromParcel(Parcel in) {
        softwareState = SoftwareState.valueOf(in.readString());
        softwareId = in.readString();
        name = in.readString();
        description = in.readString();

        installationId = in.readString();
        installationStatus = in.readString();

        downloads = in.createStringArrayList();
        downloadFilePath = in.readString();

        downloadedResources = in.createStringArrayList();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
