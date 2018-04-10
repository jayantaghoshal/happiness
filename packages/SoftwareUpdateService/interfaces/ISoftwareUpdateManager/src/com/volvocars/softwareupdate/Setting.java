/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.Parcel;
import android.os.Parcelable;

public class Setting implements Parcelable {

    public enum SettingType {
        UNDEFINED, ENABLE_OTA, AUTO_DOWNLOAD, AUTO_INSTALL
    };

    public SettingType type = SettingType.UNDEFINED;
    public boolean value = false;

    public static final Creator<Setting> CREATOR = new Creator<Setting>() {
        @Override
        public Setting createFromParcel(Parcel in) {
            return new Setting(in);
        }

        @Override
        public Setting[] newArray(int size) {
            return new Setting[size];
        }
    };

    public Setting() {
    }

    private Setting(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(type.name());
        dest.writeBoolean(value);
    }

    @Override
    public String toString() {
        String string = type + "\n" + value;
        return string;
    }

    public void readFromParcel(Parcel in) {
        type = SettingType.valueOf(in.readString());
        value = in.readBoolean();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

}