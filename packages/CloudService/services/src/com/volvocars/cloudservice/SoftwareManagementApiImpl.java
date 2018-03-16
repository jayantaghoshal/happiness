/*
 * Copyright 2017-2018 Volvo Car Corporation
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
import com.volvocars.carconfig.*;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareManagementApiImpl extends ISoftwareManagementApi.Stub {
    private static final String LOG_TAG = "CloudService.SWAPI";

    private CloudConnection cloudConnection = null;

    private String softwareManagementUri = null;
    SoftwareManagementURIs uris = null;

    private boolean softwareManagementAvailable = false;

    private ISoftwareManagementApiCallback softwareManagementApiCallback = null;
    private DownloadInfo currentDownloadInfo;

    private XmlSerializerWrapper xmlWrapper = new XmlSerializerWrapper();

    private class SwListResponse<T> {
        private ArrayList<T> swlist = new ArrayList<T>();
        private int code = -1;
    }

    private class DownloadInfoResponse {
        private DownloadInfo downloadInfo = new DownloadInfo();
        private int code = -1;
    }

    public SoftwareManagementApiImpl() {
    }

    public void init(CloudConnection cloudConnection, String softwareManagementUri) {
        this.cloudConnection = cloudConnection;
        this.softwareManagementUri = softwareManagementUri;
        softwareManagementAvailable = true;
        FetchSoftwareManagementURIs();
    }

    private void FetchSoftwareManagementURIs() {
        Log.v(LOG_TAG, "FetchSoftwareManagementURIs");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.SoftwareManagement+XML";

        headers.add(field);

        try {
            // Send request
            Log.v(LOG_TAG, "Calling doGetRequest with uri: " + softwareManagementUri);
            Response response = cloudConnection.doGetRequest(softwareManagementUri, headers, 10000);

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

            Log.v(LOG_TAG, "SoftwareManagement URIS: \n" + uris.available_software_assignments + "\n" + uris.downloads
                    + "\n" + uris.pending_installations);

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: IOException [" + ex.getMessage() + "]");
        }
    }

    private SwListResponse<SoftwareAssignment> FetchSoftwareAssignmentsList() {
        Log.v(LOG_TAG, "FetchSoftwareAssignmentsList");
        // Build request
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.AvailableUpdates+XML";
        headers.add(field);

        int timeout = 20000;

        ArrayList<SoftwareAssignment> software_list = new ArrayList();
        SwListResponse<SoftwareAssignment> swrsp = new SwListResponse();

        try {
            // Send request
            Log.v(LOG_TAG, "Calling doGetRequest with uri: " + uris.available_software_assignments);
            Response response = cloudConnection.doGetRequest(uris.available_software_assignments, headers, timeout);

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
            Log.d(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: IOException [" + ex.getMessage() + "]");
        }

        return swrsp;
    }

    private int CommissionSoftwareAssignment(String uuid) {
        Log.v(LOG_TAG, "CommissionSoftwareAssignment [" + uuid + "]");
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Commission+XML";
        headers.add(field);

        int timeout = 20000;

        Log.w(LOG_TAG, "Building body, NOTE: client_id and reason is currently hardcoded values!");
        String body = "id=" + uuid + "&client_id=" + "" + "&reason=USER"; //TODO: Remove hardcoded values (client_id and reason)

        //Do we need to fetch commission uri?
        Log.v(LOG_TAG, "Calling doPostRequest with uri: " + softwareManagementUri + " and body: " + body);
        Response response = cloudConnection.doPostRequest(softwareManagementUri + "/commission", headers, body,
                timeout);

        return response.httpResponse;

    }

    private String GetSystemLanguage() {
        Log.v(LOG_TAG, "Note: GetSystemLanguage is not tested, maybe do so?");
        switch (CarConfigApi.getValue(CarConfigEnums.CC_197_SystemLanguage.class)) {
        case Arabic:
            return "ar-BH";
        case Chinese_Simp_Man:
            return "zh-CHS";
        case Chinese_Trad_Can:
            return "zh-CH";
        case Chinese_Trad_Man:
            return "zh-CHT";
        case Czech:
            return "cs-CZ";
        case Danish:
            return "da-DK";
        case Dutch:
            return "nl-NL";
        case Australien_English:
            return "en-AU";
        case English:
            return "en-GB";
        case American_English:
            return "en-US";
        case Finnish:
            return "fi-FI";
        case Flemmish:
            return "nl-BE";
        case Canadian_French:
            return "fr-CA";
        case French:
            return "fr-FR";
        case German:
            return "de-DE";
        case Greek:
            return "el-GR";
        case Hungarian:
            return "hu-HU";
        case Italian:
            return "it-IT";
        case Japanese:
            return "ja-JP";
        case Korean_Han_Gul_:
            return "ko-KR";
        case Norwegian:
            return "nb-NO";
        case Polish:
            return "pl-PL";
        case Brazilian_Portuguese:
            return "pt-BR";
        case Portuguese:
            return "pt-PT";
        case Russian:
            return "ru-RU";
        case Spanish:
            return "es-ES";
        case American_Spanish:
            return "es-US";
        case Swedish:
            return "sv-SE";
        case Thai:
            return "th-TH";
        case Turkish:
            return "tr-TR";
        case Bulgarian:
            return "bg-BG";
        case Estonian:
            return "et-EE";
        case Latvian:
            return "lv-LV";
        case Lithuanian:
            return "lt-LT";
        case Romanian:
            return "ro-RO";
        case Slovak:
            return "sk-SK";
        case Slovene:
            return "sl-SI";
        default:
            return "";
        }
    }

    private SwListResponse<InstallationOrder> FetchPendingInstallations() {
        Log.v(LOG_TAG, "FetchPendingInstallations");
        // Build request
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();
        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.PendingInstallations+XML";
        headers.add(field);

        String systemLanguage = GetSystemLanguage();
        Log.v(LOG_TAG, "SystemLanguage: " + systemLanguage);

        if (!systemLanguage.isEmpty()) {
            HttpHeaderField languagefield = new HttpHeaderField();
            languagefield.name = "Accept-Language";
            languagefield.value = systemLanguage;
            headers.add(languagefield);
        }

        int timeout = 20000;

        ArrayList<InstallationOrder> installationOrder = new ArrayList();
        SwListResponse<InstallationOrder> swrsp = new SwListResponse();

        try {
            // Send request
            Log.v(LOG_TAG, "Calling doGetRequest with uri: " + uris.pending_installations);
            Response response = cloudConnection.doGetRequest(uris.pending_installations, headers, timeout);

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
            installationOrder = XmlParser.ParsePendingInstallations(stream);

            swrsp.swlist = installationOrder;
            swrsp.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.d(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return swrsp;
    }

    private DownloadInfoResponse FetchDownloadInfo(String uuid) {
        Log.v(LOG_TAG, "FetchDownloadInfo");
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Downloads+XML";
        headers.add(field);

        int timeout = 20000;

        DownloadInfoResponse downloadInfoResponse = new DownloadInfoResponse();

        try {
            // Send request
            Log.v(LOG_TAG, "Calling doGetRequest with uri: " + uris.downloads + "and query: ?id=" + uuid);
            Response response = cloudConnection.doGetRequest(uris.downloads + "?id=" + uuid, headers, timeout);

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

            downloadInfoResponse.downloadInfo = XmlParser.ParseDownloadInfo(stream);
            downloadInfoResponse.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG, "Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return downloadInfoResponse;
    }

    private void FetchDownloadData() {
        Log.v(LOG_TAG, "FetchDownloadData");
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        //Todo: Content-Disposition header?
        Log.v(LOG_TAG, "Should Content-Disposition header be included in downloadRequest? (Not implemented)");

        int timeout = 20000;

        // Cannot arrive here if resourceUris is empty, safe to ignore IndexOutOfBoundException
        String uri = currentDownloadInfo.resourceUris.get(0);

        // If resource URI does not contain any '/' an IndexOutOfBoundException will be thrown.
        Log.w(LOG_TAG, "FetchDownloadData: Currently ignoring potential IndexOutOfBoundException");
        String filepath = "/data/vendor/ota/" + currentDownloadInfo.installationOrderId + uri.substring(uri.lastIndexOf("/"));

        Log.v(LOG_TAG, "Calling downloadRequest with uri: \"" + uri + "\" and filepath: \"" + filepath + "\"");
        cloudConnection.downloadRequest(uri, headers, filepath, timeout);
    }

    private void FetchNextDownloadData() {
        currentDownloadInfo.downloadedResources.add(currentDownloadInfo.resourceUris.get(0));
        currentDownloadInfo.resourceUris.remove(0);

        if (currentDownloadInfo.resourceUris.isEmpty()) {
            Log.v(LOG_TAG, "Download finished...");

            try {
                softwareManagementApiCallback.DownloadData(200, currentDownloadInfo);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG,
                        "Something went bananas with DownloadData callback: RemoteException [" + ex.getMessage() + "]");
            }
            softwareManagementApiCallback = null;
            currentDownloadInfo = null;
        } else {
            Log.v(LOG_TAG, "Downloading next file...");

            try {
                softwareManagementApiCallback.DownloadData(200, currentDownloadInfo);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG,
                        "Something went bananas with DownloadData callback: RemoteException [" + ex.getMessage() + "]");
            }

            FetchDownloadData();
        }
    }

    public void downloadStatusUpdate(Response response) {
        //Check if download_id match currentDownloadId?

        Log.v(LOG_TAG, "File download finished with response " + response.httpResponse);

        if (response.httpResponse != 200) {
            Log.d(LOG_TAG, "Download failed with error code: " + response.httpResponse);
            try {
                softwareManagementApiCallback.DownloadData(response.httpResponse, currentDownloadInfo);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG,
                        "Something went bananas with DownloadData callback: RemoteException [" + ex.getMessage() + "]");
            }
            softwareManagementApiCallback = null;
            currentDownloadInfo = null;
        } else {
            FetchNextDownloadData();
        }
    }

    private int postInstallNotification(InstallNotification notification) {
        Log.v(LOG_TAG,
                "notification: [softwareid: " + notification.softwareId + ", installationOrderId: "
                        + notification.installationOrderId + ", StatusCode: "
                        + notification.notification.status.statusCode.toString() + ", SubStatusCode: "
                        + notification.notification.status.subStatusCode.toString() + ", SubStatusReason: "
                        + notification.notification.status.subStatusReason.toString() + "]");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.InstallNotification+XML";
        headers.add(field);

        int timeout = 20000;

        String body = xmlWrapper.serializeInstallNotification(notification);

        Log.v(LOG_TAG, "Calling doPostRequest with uri: " + softwareManagementUri + " and body: " + body);
        Response response = cloudConnection.doPostRequest(softwareManagementUri + "/installNotification", headers, body,
                timeout); //TODO: retrieve uri from software?

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

        if (!softwareManagementAvailable) {
            callback.SoftwareAssignmentList(-1, null);
        }

        SwListResponse<SoftwareAssignment> swrsp = FetchSoftwareAssignmentsList();
        callback.SoftwareAssignmentList(swrsp.code, swrsp.swlist);
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     */
    @Override
    public void CommissionSoftwareAssignment(String uuid, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.CommissionStatus(uuid, -1);
        }

        callback.CommissionStatus(uuid, CommissionSoftwareAssignment(uuid));
    }

    /**
    * Get a list of pending installations (installation orders)
    * @param callback
    */
    @Override
    public void GetPendingInstallations(ISoftwareManagementApiCallback callback) throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.PendingInstallations(-1, null);
        }

        SwListResponse<InstallationOrder> swrsp = FetchPendingInstallations();
        callback.PendingInstallations(swrsp.code, swrsp.swlist);
    }

    /**
    * Get Download Info for an installation order
    * @param uuid installation order id
    * @param callback
    */
    @Override
    public void GetDownloadInfo(String uuid, ISoftwareManagementApiCallback callback) throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.DownloadInfo(-1, null);
        }

        DownloadInfoResponse downloadInfoResponse = FetchDownloadInfo(uuid);
        callback.DownloadInfo(downloadInfoResponse.code, downloadInfoResponse.downloadInfo);
    }

    /**
    * Get Download data
    * @param downloadInfo Contains information of what to be downloaded
    * @param callback     Callback to be called when the status of the download changes
    */
    public void GetDownloadData(DownloadInfo downloadInfo, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.DownloadData(-1, null);
            return;
        }

        //TODO: Verify that no download is in progress already, if so... call callback function with some busy info...
        if (softwareManagementApiCallback != null) {
            Log.w(LOG_TAG, "Download already in progress, try later...");
            callback.DownloadData(-1, null);
            return;
        }

        Log.v(LOG_TAG, "Download requested, try to download " + downloadInfo.resourceUris.size() + " files");

        softwareManagementApiCallback = callback;
        currentDownloadInfo = downloadInfo;

        FetchDownloadData();
    }

    /**
     * Post InstallationReport
     * @param installationReport Report to be posted
     * @param callback           Callback to be called when the status of the download changes
     */
    public void PostInstallationReport(InstallationReport installationReport, ISoftwareManagementApiCallback callback) {

    }

    /**
    * Post InstallNotification
    * @param notification InstallNotification to be posted
    * @param callback     Callback to be called
    */
    public void PostInstallNotification(InstallNotification notification, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.InstallNotificationStatus(-1, notification.installationOrderId);
            return;
        }
        callback.InstallNotificationStatus(postInstallNotification(notification), notification.installationOrderId);
    }
}