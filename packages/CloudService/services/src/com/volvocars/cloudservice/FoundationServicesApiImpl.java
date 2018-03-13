/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.RemoteException;
import android.os.SystemProperties;

import android.util.Log;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.IOException;

import java.util.ArrayList;

import org.xmlpull.v1.XmlPullParserException;
import android.os.SystemProperties;

import vendor.volvocars.hardware.cloud.V1_0.*;

/**
 * Implementation of Foundation service API.
 */
public class FoundationServicesApiImpl extends IFoundationServicesApi.Stub {
    private CloudConnection server = null;
    private static final String LOG_TAG = "CloudService.FsApi";

    private boolean foundation_services_is_available = false;

    public FoundationServicesApiImpl() {

    }

    public void init(CloudConnection server) {
        this.server = server;
        foundation_services_is_available = true;
    }

    public Feature getFeatureAvailable(String feature) {
        Log.v(LOG_TAG, "getFeatureAvailable [" + feature + "]");
        //Setup request fields
        HttpHeaderField field = new HttpHeaderField();
        field.value = "application/volvo.cloud.Features+XML";
        field.name = "Accept";
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();
        headers.add(field);
        String uri = "features-1";
        int timeout = 20000;
        //DoRequest
        try {
            Response response = server.doGetRequest(uri, headers, timeout);
            byte[] bytesdata = new byte[response.responseData.size()];

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.d(LOG_TAG, "Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            InputStream stream = new ByteArrayInputStream(bytesdata);
            //ParseData
            ArrayList<Feature> features = XmlParser.parse(stream);
            for (Feature f : features) {
                if (f.name.equals(feature))
                    return f;
            }
        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return null;
    }

    /**
    * Performs request against cloudd.
    * Parses the response using xmlparser.
    * Searches for feature in arraylist structure.
    * @param feature string indicating which feature wanted.
    * @return boolean indicating if the feature exist.
    */
    @Override
    public boolean IsFeatureAvailable(String feature) throws RemoteException {
        if (!foundation_services_is_available) {
            return false;
        }
        return (getFeatureAvailable(feature) != null);
    }

    private boolean HandleHttpResponseCode(final int code) {
        return code == 200;
    }
}
