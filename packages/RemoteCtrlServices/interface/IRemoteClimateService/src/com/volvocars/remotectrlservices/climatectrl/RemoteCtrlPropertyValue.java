/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl;

import static java.lang.Integer.toHexString;

import android.annotation.SystemApi;
import android.os.Parcel;
import android.os.Parcelable;

import java.nio.charset.Charset;

/**
 * Stores values broken down by area for a vehicle property.
 *
 * @param <T> refer to Parcel#writeValue(Object) to get a list of all supported types. The class
 * should be visible to framework as default class loader is being used here.
 *
 * @hide
 */
@SystemApi
public class RemoteCtrlPropertyValue<T> implements Parcelable {

    private final static Charset DEFAULT_CHARSET = Charset.forName("UTF-8");

    private final int mPropertyId;
    private final int mAreaId;
    private final int mStatus;
    private final T mValue;

    public RemoteCtrlPropertyValue(int propertyId, int areaId, T value) {
        this(propertyId, areaId, 0, value);
    }

    public RemoteCtrlPropertyValue(int propertyId, int areaId, int status, T value) {
        mPropertyId = propertyId;
        mAreaId = areaId;
        mStatus = status;
        mValue = value;
    }

    @SuppressWarnings("unchecked")
    public RemoteCtrlPropertyValue(Parcel in) {
        mPropertyId = in.readInt();
        mAreaId = in.readInt();
        mStatus = in.readInt();
        String valueClassName = in.readString();
        Class<?> valueClass;
        try {
            valueClass = Class.forName(valueClassName);
        } catch (ClassNotFoundException e) {
            throw new IllegalArgumentException("Class not found: " + valueClassName);
        }

        if (String.class.equals(valueClass)) {
            byte[] bytes = in.readBlob();
            mValue = (T) new String(bytes, DEFAULT_CHARSET);
        } else if (byte[].class.equals(valueClass)) {
            mValue = (T) in.readBlob();
        } else {
            mValue = (T) in.readValue(valueClass.getClassLoader());
        }
    }

    public static final Creator<RemoteCtrlPropertyValue> CREATOR = new Creator<RemoteCtrlPropertyValue>() {
        @Override
        public RemoteCtrlPropertyValue createFromParcel(Parcel in) {
            return new RemoteCtrlPropertyValue(in);
        }

        @Override
        public RemoteCtrlPropertyValue[] newArray(int size) {
            return new RemoteCtrlPropertyValue[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mPropertyId);
        dest.writeInt(mAreaId);
        dest.writeInt(mStatus);

        Class<?> valueClass = mValue == null ? null : mValue.getClass();
        dest.writeString(valueClass == null ? null : valueClass.getName());

        // Special handling for String and byte[] to mitigate transaction buffer limitations.
        if (String.class.equals(valueClass)) {
            dest.writeBlob(((String)mValue).getBytes(DEFAULT_CHARSET));
        } else if (byte[].class.equals(valueClass)) {
            dest.writeBlob((byte[]) mValue);
        } else {
            dest.writeValue(mValue);
        }
    }

    public int getPropertyId() {
        return mPropertyId;
    }

    public int getAreaId() {
        return mAreaId;
    }

    public int getStatus() {
        return mStatus;
    }

    public T getValue() {
        return mValue;
    }

    @Override
    public String toString() {
        return "RemoteCtrlPropertyValue{" +
                "mPropertyId=0x" + toHexString(mPropertyId) +
                ", mAreaId=0x" + toHexString(mAreaId) +
                ", mStatus=" + mStatus +
                ", mValue=" + mValue +
                '}';
    }
}
