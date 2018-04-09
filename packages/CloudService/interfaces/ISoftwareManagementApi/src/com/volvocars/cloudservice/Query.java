/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class Query implements Parcelable {
    private final String installationOrderParam = "installation_order_id=";
    private final String typeParam = "type=";
    private final String idParam = "id=";
    private final String softwareProductParam = "software_product_id=";

    public String type = "";
    public String id = "";
    public String softwareProductId = "";
    public String installationOrderId = "";

    public static final Creator<Query> CREATOR = new Creator<Query>() {
        @Override
        public Query createFromParcel(Parcel in) {
            return new Query(in);
        }

        @Override
        public Query[] newArray(int size) {
            return new Query[size];
        }
    };

    public Query() {
    }

    private Query(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(type);
        dest.writeString(id);
        dest.writeString(softwareProductId);
        dest.writeString(installationOrderId);
    }

    @Override
    public String toString() {
        String string = type + "\n" + id + "\n" + softwareProductId + "\n" + installationOrderId;
        return string;
    }

    public void readFromParcel(Parcel in) {
        type = in.readString();
        id = in.readString();
        softwareProductId = in.readString();
        installationOrderId = in.readString();
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }

    public String buildQuery() {
        String query = "?";

        if (!type.isEmpty()) {
            query += typeParam + type + "&";
        }
        if (!id.isEmpty()) {
            query += idParam + id + "&";
        }
        if (!softwareProductId.isEmpty()) {
            query += softwareProductParam + softwareProductId + "&";
        }
        if (!installationOrderId.isEmpty()) {
            query += installationOrderParam + installationOrderId + "&";
        }

        if (query.endsWith("&")) query = query.substring(0, query.length() - 1);
        if (1 == query.length()) query = "";

        return query;
    }
}