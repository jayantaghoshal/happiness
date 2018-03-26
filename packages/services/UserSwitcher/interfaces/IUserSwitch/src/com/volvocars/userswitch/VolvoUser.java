/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.os.Parcel;
import android.os.Parcelable;
import android.os.SystemProperties;
import android.os.UserHandle;

/**
 * User information
 * Android User Info is shortened
 */

public class VolvoUser implements Parcelable {
    public int id;
    public String name;
    public int flags;

    public static final int FLAG_MASK_USER_TYPE = 255;
    public static final int FLAG_PRIMARY = 1;
    public static final int FLAG_ADMIN = 2;
    public static final int FLAG_GUEST = 4;
    public static final int FLAG_RESTRICTED = 8;
    public static final int FLAG_INITIALIZED = 16;
    public static final int FLAG_MANAGED_PROFILE = 32;
    public static final int FLAG_DISABLED = 64;


    public static final Creator<VolvoUser> CREATOR = new Creator<VolvoUser>() {
        public VolvoUser createFromParcel(Parcel source) {
            return new VolvoUser(source);
        }

        public VolvoUser[] newArray(int size) {
            return new VolvoUser[size];
        }
    };

    public VolvoUser(int id) {
        this(id, "", 0);
    }

    public VolvoUser(int id, String name, int flags) {
        this.id = id;
        this.name = name;
        this.flags = flags;
    }

    public boolean isPrimary() {
        return (this.flags & 1) == 1;
    }

    public boolean isAdmin() {
        return (this.flags & 2) == 2;
    }

    public boolean isGuest() {
        return (this.flags & 4) == 4;
    }

    public boolean isRestricted() {
        return (this.flags & 8) == 8;
    }

    public boolean isManagedProfile() {
        return (this.flags & 32) == 32;
    }

    public boolean isEnabled() {
        return (this.flags & 64) != 64;
    }

    public String toString() {
        return "VolvoUser{" + this.id + ":" + this.name + ":" + Integer.toHexString(this.flags) + "}";
    }

    public int describeContents() {
        return 0;
    }

    public void writeToParcel(Parcel dest, int parcelableFlags) {
        dest.writeInt(this.id);
        dest.writeString(this.name);
        dest.writeInt(this.flags);

    }

    private VolvoUser(Parcel source) {
        this.id = source.readInt();
        this.name = source.readString();
        this.flags = source.readInt();
    }
}