/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

import android.util.Log;
import android.os.RemoteException;
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
public class SoftwareManagementAPI extends ISoftwareManagementAPI.Stub {
    private static final String LOG_TAG = "CloudService.SWAPI";

    private ICloudConnection cloud_connection_service;
    private boolean software_management_available;

    public SoftwareManagementAPI(ICloudConnection cloud_connection_service) {
        this.cloud_connection_service = cloud_connection_service;

        // Need to be aware somehow that SoftwareManagement is available or not?
    }

    /**
     * Get a the list of available assignments of a specific type
     * @param callback
     */
    @Override
    public void GetSoftwareAssigmentList(ISoftwareManagementAPICallback callback) throws RemoteException {
        // Build request

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "ACCEPT";
        field.value = "application/volvo.cloud.AvailableAssignments+XML";

        headers.add(field);

        String uri = "available-assignments-1";

        boolean useHttps = true;
        int timeout = 15;

        ArrayList<SoftwareAssignment> software_list = new ArrayList();

        try {
            // Send request
            Response response = cloud_connection_service.doGetRequest(uri, headers, useHttps, timeout);

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            InputStream stream = new ByteArrayInputStream(bytesdata);
            software_list = XmlParser.ParseSoftwareAssignments(stream);
        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "ex: " + ex.getMessage());
        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "ex: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "ex: " + ex.getMessage());
        }

        callback.SoftwareAssignmentList(400, software_list);
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     */
    @Override
    public void CommissionSoftwareAssignment(String uuid, ISoftwareManagementAPICallback callback)
            throws RemoteException {
        callback.CommissionStatus(400);
    }
}