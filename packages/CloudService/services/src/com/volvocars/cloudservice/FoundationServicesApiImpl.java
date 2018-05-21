/*
 * Copyright 2017-2018 Volvo Car Corporation
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
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

import org.xmlpull.v1.XmlPullParserException;
import android.os.SystemProperties;

import vendor.volvocars.hardware.cloud.V1_0.*;
import com.volvocars.cloudservice.parser.FeaturesParser;

/**
 * Implementation of Foundation service API.
 */
public class FoundationServicesApiImpl extends IFoundationServicesApi.Stub {
    private CloudConnection server = null;
    private static final String LOG_TAG = "CloudService.FsApi";

    private boolean foundation_services_is_available = false;

    private ArrayList<Feature> features = new ArrayList();

    private ExecutorService executorService = Executors.newSingleThreadExecutor();

    public FoundationServicesApiImpl() {

    }

    public void init(CloudConnection server) {
        this.server = server;
        foundation_services_is_available = true;
    }

    public boolean getFeatureList() {
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

            //Parse XML data
            features = FeaturesParser.parse(stream);

            return true;
        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return false;
    }

    /**
    * Performs request against cloudd.
    * Parses the response using features.
    * Searches for feature in arraylist structure.
    * @param feature string indicating which feature wanted.
    * @return boolean indicating if the feature exist.
    */
    @Override
    public boolean isFeatureAvailable(String feature, IFoundationServicesApiCallback callback) throws RemoteException {
        Log.v(LOG_TAG, "isFeatureAvailable [" + feature + "]");

        if (!foundation_services_is_available) {
            return false;
        }

        executorService.execute( () -> {
            //If feature list isn't retrieved yet, get it now
            if (features.size() == 0) {
                getFeatureList();
            }

            Feature theFeature = null;
            for (Feature f : features) {
                if (f.name.equals(feature)) {
                    theFeature = f;
                    break;
                }
            }

            try {
                callback.featureAvailableResponse(theFeature);
            }
            catch (RemoteException e) {
                Log.e(LOG_TAG, "featureAvailableResponse callback failed (" + e.getMessage() + ")");
            }
        });

        return true;
    }

    private boolean HandleHttpResponseCode(final int code) {
        return code == 200;
    }
}
