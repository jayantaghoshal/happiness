/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.ArrayList;

public class Feature implements Parcelable {

    public String name = "";
    public boolean enabled = false;
    public boolean visible = false;
    public String uri = "";
    public String iconUri = "";
    public String description = "";
    public ArrayList<String> tags = new ArrayList();

    public static final Creator<Feature> CREATOR = new Creator<Feature>() {
        @Override
        public Feature createFromParcel(Parcel in) {
            return new Feature(in);
        }

        @Override
        public Feature[] newArray(int size) {
            return new Feature[size];
        }
    };

    public Feature() {
    }

    private Feature(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(name);
        dest.writeBoolean(enabled);
        dest.writeBoolean(visible);
        dest.writeString(uri);
        dest.writeString(iconUri);
        dest.writeString(description);
        dest.writeStringList(tags);
    }

    @Override
    public String toString() {
        String string = name + "\n" + enabled + "\n" + visible + "\n" + uri.toString() + "\n" + description.toString();
        return string;
    }

    public void readFromParcel(Parcel in) {
        name = in.readString();
        enabled = in.readBoolean();
        visible = in.readBoolean();
        uri = in.readString();
        iconUri = in.readString();
        description = in.readString();
        tags = in.createStringArrayList();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}