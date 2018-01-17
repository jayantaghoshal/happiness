/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

import android.os.Parcel;
import android.os.Parcelable;

public class SoftwareAssignment implements Parcelable {
    private String uuid;
    private String name;
    private String description;
    private byte[] image;

    public static final Creator<SoftwareAssignment> CREATOR = new Creator<SoftwareAssignment>() {
        @Override
        public SoftwareAssignment createFromParcel(Parcel in) {
            return new SoftwareAssignment(in);
        }

        @Override
        public SoftwareAssignment[] newArray(int size) {
            return new SoftwareAssignment[size];
        }
    };

    public SoftwareAssignment() {
    }

    private SoftwareAssignment(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(uuid);
        dest.writeString(name);
        dest.writeString(description);

        dest.writeInt(image.length);
        for (byte b : image) {
            dest.writeByte(b);
        }
    }

    @Override
    public String toString() {
        String string = uuid + "\n" + name + "\n" + description + "\n";
        return string;
    }

    public void readFromParcel(Parcel in) {
        uuid = in.readString();
        name = in.readString();
        description = in.readString();

        int image_size = in.readInt();
        image = new byte[image_size];

        for (int i = 0; i < image_size; i++) {
            image[i] = in.readByte();
        }
    }

    @Override
    public int describeContents() {
        // As long there are no children, this function is quite useless?
        return 0;
    }
}
