/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.util.Log;
import android.os.RemoteException;
import android.os.SystemProperties;
import vendor.volvocars.hardware.cloud.V1_0.*;
import java.util.ArrayList;
import java.util.List;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.IOException;
import org.xmlpull.v1.XmlPullParserException;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareManagementApiImpl extends ISoftwareManagementApi.Stub {
    private static final String LOG_TAG = "CloudService.SWAPI";

    private CloudConnection cloud_connection = null;

    private String softwareManagementUri = null;
    SoftwareManagementURIs uris = null;

    private boolean software_management_available = false;

    private class SwListResponse {
        private ArrayList<SoftwareAssignment> swlist = new ArrayList<SoftwareAssignment>();
        private int code = -1;
    }

    public SoftwareManagementApiImpl() {
    }

    public void init(CloudConnection cloud_connection, String softwareManagementUri) {
        this.cloud_connection = cloud_connection;
        this.softwareManagementUri = softwareManagementUri;
        software_management_available = true;
        FetchSoftwareManagementURIs();
    }

    private void FetchSoftwareManagementURIs() {
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.SoftwareManagement+XML";

        headers.add(field);

        try {
            // Send request
            Response response = cloud_connection.doGetRequest(softwareManagementUri, headers, 10000);

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.w(LOG_TAG, "Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            String s = new String(bytesdata);
            Log.e(LOG_TAG, s);

            InputStream stream = new ByteArrayInputStream(bytesdata);
            uris = XmlParser.ParseSoftwareManagementURIs(stream);
            stream.close();

            Log.v(LOG_TAG, "SoftwareManagement URIS: \n" + uris.available_software_assignments + "\n" + uris.downloads);

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Something went bananas with the parsing: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: " + ex.getMessage());
        }
    }

    private SwListResponse FetchSoftwareAssignmentsList() {
        // Build request
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.AvailableUpdates+XML";
        headers.add(field);

        int timeout = 20000;

        ArrayList<SoftwareAssignment> software_list = new ArrayList();
        SwListResponse swrsp = new SwListResponse();

        try {
            // Send request
            Response response = cloud_connection.doGetRequest(uris.available_software_assignments, headers, timeout);

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.w(LOG_TAG, "Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            InputStream stream = new ByteArrayInputStream(bytesdata);
            software_list = XmlParser.ParseSoftwareAssignments(stream);

            swrsp.swlist = software_list;
            swrsp.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Something went bananas with the parsing: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: " + ex.getMessage());
        }

        return swrsp;
    }

    private int CommissionSoftwareAssignment(String uuid) {

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Commission+XML";
        headers.add(field);

        int timeout = 20000;

        String body = "id=" + uuid + "&client_id=" + "" + "&reason=USER"; //TODO: Remove hardcoded values (client_id and reason)

        //Do we need to fetch commission uri?
        Response response = cloud_connection.doPostRequest(softwareManagementUri + "/commission", headers, body,
                timeout);

        return response.httpResponse;

    }

    private boolean HandleHttpResponseCode(final int code) {

        return code == 200;
    }

    /**
     * Get a the list of available assignments of a specific type
     * @param callback
     */
    @Override
    public void GetSoftwareAssigmentList(ISoftwareManagementApiCallback callback) throws RemoteException {

        if (!software_management_available) {
            callback.SoftwareAssignmentList(-1, null);
        }

        SwListResponse swrsp = FetchSoftwareAssignmentsList();
        callback.SoftwareAssignmentList(swrsp.code, swrsp.swlist);
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     */
    @Override
    public void CommissionSoftwareAssignment(String uuid, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!software_management_available) {
            callback.CommissionStatus(-1);
        }

        callback.CommissionStatus(CommissionSoftwareAssignment(uuid));
    }
}