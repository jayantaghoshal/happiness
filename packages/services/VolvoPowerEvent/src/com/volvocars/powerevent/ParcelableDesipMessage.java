package com.volvocars.powerevent;

import android.os.Parcel;
import android.os.Parcelable;

public class ParcelableDesipMessage implements Parcelable {
    private byte aid;
    private byte fid;
    private byte[] data;

    protected ParcelableDesipMessage(Parcel in) {
        aid = in.readByte();
        fid = in.readByte();

        int dataSize = in.readInt();
        data = new byte[dataSize];

        for (int i = 0; i < dataSize; i++) {
            data[i] = in.readByte();
        }
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        //TODO: implement write to parcel

        // dest.writeByte(aid);
        // dest.writeByte(fid);

        // Probably need to write the data manually rather than using writeByteArray()
        // dest.writeInt(dataSize); /// this might be performed by write byte array below.
        // dest.writeByteArray(data);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<ParcelableDesipMessage> CREATOR = new Creator<ParcelableDesipMessage>() {
        @Override
        public ParcelableDesipMessage createFromParcel(Parcel in) {
            return new ParcelableDesipMessage(in);
        }

        @Override
        public ParcelableDesipMessage[] newArray(int size) {
            return new ParcelableDesipMessage[size];
        }
    };
}
