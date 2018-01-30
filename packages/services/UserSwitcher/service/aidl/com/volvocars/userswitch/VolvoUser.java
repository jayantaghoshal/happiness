/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * This class represents the user information
 */
public class VolvoUser implements Parcelable {

    public static final Creator<VolvoUser> CREATOR = new Creator<VolvoUser>() {
        @Override
        public VolvoUser[] newArray(int size) {
            return new VolvoUser[size];
        }

        @Override
        public VolvoUser createFromParcel(Parcel source) {
            return new VolvoUser(source);
        }
    };
    private String name;
    private int id;
    private int volvoProfile;

    public VolvoUser(Parcel source) {
        name = source.readString();
        id = source.readInt();
        volvoProfile = source.readInt();
    }

    public VolvoUser(String name, int id, int volvoProfile) {
        this.name = name;
        this.id = id;
        this.volvoProfile = volvoProfile;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getIdString() {
        return String.valueOf(id);
    }

    public int getVolvoProfile() {
        return volvoProfile;
    }

    public void setVolvoProfile(int volvoProfile) {
        this.volvoProfile = volvoProfile;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {

        dest.writeString(name);
        dest.writeInt(id);
        dest.writeInt(volvoProfile);
    }

    @Override
    public String toString() {
        return "name: " + name + "  androidId: " + id + "  volvoId: " + volvoProfile;
    }
}