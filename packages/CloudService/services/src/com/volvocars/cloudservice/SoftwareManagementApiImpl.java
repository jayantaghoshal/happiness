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
import com.volvocars.cloudservice.parser.XmlParser;
import com.volvocars.cloudservice.parser.softwaremanagement.DownloadInfoParser;
import com.volvocars.cloudservice.parser.softwaremanagement.InstallNotificationParser;
import com.volvocars.cloudservice.parser.softwaremanagement.SoftwareAssignmentParser;
import com.volvocars.cloudservice.parser.softwaremanagement.SoftwareManagementParser;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareManagementApiImpl extends ISoftwareManagementApi.Stub {
    private static final String LOG_TAG = "CloudService";
    private static final String LOG_PREFIX = "[SoftwareManagementApiImpl]";

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

    private class InstallNotificationResponse {
        private InstallNotification notification = new InstallNotification();
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
        Log.v(LOG_TAG, LOG_PREFIX + " FetchSoftwareManagementURIs");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.SoftwareManagement+XML";

        headers.add(field);

        try {
            // Send request
            Log.v(LOG_TAG, LOG_PREFIX + " Calling doGetRequest with uri: " + softwareManagementUri);
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

            Log.v(LOG_TAG, LOG_PREFIX + " Response: " + new String(bytesdata));

            InputStream stream = new ByteArrayInputStream(bytesdata);
            uris = SoftwareManagementParser.ParseSoftwareManagementURIs(stream);
            stream.close();

            Log.v(LOG_TAG, LOG_PREFIX + " SoftwareManagement URIS: \n" + uris.available_updates + "\n"
                    + uris.available_accessories + "\n" + uris.downloads + "\n" + uris.pending_installations);

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Something went bananas with the streams: IOException [" + ex.getMessage() + "]");
        }
    }

    private SwListResponse<SoftwareAssignment> FetchSoftwareAssignment(Query query, AssignmentType type) {
        Log.v(LOG_TAG, LOG_PREFIX + " FetchSoftwareAssignmentsList");
        // Build request
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.AvailableUpdates+XML";
        headers.add(field);

        String systemLanguage = GetSystemLanguage();
        Log.v(LOG_TAG, LOG_PREFIX + " SystemLanguage: " + systemLanguage);

        if (!systemLanguage.isEmpty()) {
            HttpHeaderField languagefield = new HttpHeaderField();
            languagefield.name = "Accept-Language";
            languagefield.value = systemLanguage;
            headers.add(languagefield);
        }

        int timeout = 20000;

        ArrayList<SoftwareAssignment> software_list = new ArrayList();
        SwListResponse<SoftwareAssignment> swrsp = new SwListResponse();

        try {
            // Send request
            String uri = (type == AssignmentType.UPDATE) ? uris.available_updates : uris.available_accessories;

            Log.v(LOG_TAG, LOG_PREFIX + " Calling doGetRequest with uri: " + uri);
            Response response = null;

            response = cloudConnection.doGetRequest(uri + query.buildQuery(), headers, timeout);

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.w(LOG_TAG, LOG_PREFIX + " Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            Log.v(LOG_TAG, LOG_PREFIX + " Response: " + new String(bytesdata));

            InputStream stream = new ByteArrayInputStream(bytesdata);
            software_list = SoftwareAssignmentParser.ParseSoftwareAssignments(stream);

            swrsp.swlist = software_list;
            swrsp.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.d(LOG_TAG,
                    LOG_PREFIX + " Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Something went bananas with the streams: IOException [" + ex.getMessage() + "]");
        }

        return swrsp;
    }

    private int CommissionSoftwareAssignment(CommissionElement commissionElement) {
        Log.v(LOG_TAG,
                LOG_PREFIX + " CommissionSoftwareAssignment [id: " + commissionElement.id + ", reason:"
                        + commissionElement.reason + ", action:" + commissionElement.action + ", client_id: "
                        + commissionElement.clientId + ", uri: " + commissionElement.commissionUri + "]");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Commission+XML";
        headers.add(field);

        int timeout = 20000;

        String body = xmlWrapper.serializeCommissionElement(commissionElement);

        Log.v(LOG_TAG,
                LOG_PREFIX + " Calling doPostRequest with uri: " + softwareManagementUri
                        + (commissionElement.commissionUri.startsWith("/") ? commissionElement.commissionUri
                                : "/" + commissionElement.commissionUri)
                        + " and body: " + body);
        Response response = cloudConnection.doPostRequest(softwareManagementUri
                + (commissionElement.commissionUri.startsWith("/") ? commissionElement.commissionUri
                        : "/" + commissionElement.commissionUri),
                headers, body, timeout);

        return response.httpResponse;

    }

    private String GetSystemLanguage() {
        Log.v(LOG_TAG, LOG_PREFIX + " Note: GetSystemLanguage is not tested, maybe do so?");
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

    private DownloadInfoResponse FetchDownloadInfo(InstallationOrder installationOrder) {
        Log.v(LOG_TAG, LOG_PREFIX + " FetchDownloadInfo");
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Downloads+XML";
        headers.add(field);

        int timeout = 20000;

        DownloadInfoResponse downloadInfoResponse = new DownloadInfoResponse();

        try {
            // Send request
            Log.v(LOG_TAG, LOG_PREFIX + " Calling doGetRequest with uri: " + softwareManagementUri
                    + installationOrder.downloadsUri);
            Response response = cloudConnection.doGetRequest(softwareManagementUri + installationOrder.downloadsUri,
                    headers, timeout);

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.w(LOG_TAG, LOG_PREFIX + " Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            InputStream stream = new ByteArrayInputStream(bytesdata);

            downloadInfoResponse.downloadInfo = DownloadInfoParser.ParseDownloadInfo(stream);
            downloadInfoResponse.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, LOG_PREFIX + " Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return downloadInfoResponse;
    }

    private void FetchDownloadData() {
        Log.v(LOG_TAG, LOG_PREFIX + " FetchDownloadData");
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        // Todo: Content-Disposition header?
        Log.v(LOG_TAG,
                LOG_PREFIX + " Should Content-Disposition header be included in downloadRequest? (Not implemented)");

        int timeout = 20000;

        // Cannot arrive here if resourceUris is empty, safe to ignore
        // IndexOutOfBoundException
        String uri = currentDownloadInfo.resourceUris.get(0);

        // If resource URI does not contain any '/' an IndexOutOfBoundException will be
        // thrown.
        Log.w(LOG_TAG, LOG_PREFIX + " FetchDownloadData: Currently ignoring potential IndexOutOfBoundException");
        String filepath = "/data/vendor/ota/" + currentDownloadInfo.installationOrderId
                + uri.substring(uri.lastIndexOf("/"));

        Log.v(LOG_TAG,
                LOG_PREFIX + " Calling downloadRequest with uri: \"" + uri + "\" and filepath: \"" + filepath + "\"");
        cloudConnection.downloadRequest(uri, headers, filepath, timeout);
    }

    private void FetchNextDownloadData() {
        currentDownloadInfo.downloadedResources.add(currentDownloadInfo.resourceUris.get(0));
        currentDownloadInfo.resourceUris.remove(0);

        if (currentDownloadInfo.resourceUris.isEmpty()) {
            Log.v(LOG_TAG, LOG_PREFIX + " Download finished...");

            try {
                softwareManagementApiCallback.DownloadData(200, currentDownloadInfo);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG, LOG_PREFIX + " Something went bananas with DownloadData callback: RemoteException ["
                        + ex.getMessage() + "]");
            }
            softwareManagementApiCallback = null;
            currentDownloadInfo = null;
        } else {
            Log.v(LOG_TAG, LOG_PREFIX + " Downloading next file...");

            FetchDownloadData();
        }
    }

    public void downloadStatusUpdate(Response response) {
        // Check if download_id match currentDownloadId?

        Log.v(LOG_TAG, LOG_PREFIX + " File download finished with response " + response.httpResponse);

        if (response.httpResponse != 200) {
            Log.d(LOG_TAG, LOG_PREFIX + " Download failed with error code: " + response.httpResponse);
            try {
                softwareManagementApiCallback.DownloadData(response.httpResponse, currentDownloadInfo);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG, LOG_PREFIX + " Something went bananas with DownloadData callback: RemoteException ["
                        + ex.getMessage() + "]");
            }
            softwareManagementApiCallback = null;
            currentDownloadInfo = null;
        } else {
            FetchNextDownloadData();
        }
    }

    private int postInstallNotification(InstallNotification notification) {
        Log.v(LOG_TAG, LOG_PREFIX + " notification: [softwareid: " + notification.softwareId + ", installationOrderId: "
                + notification.installationOrderId + ", StatusCode: "
                + notification.notification.status.statusCode.toString() + ", SubStatusCode: "
                + notification.notification.status.subStatusCode.toString() + ", SubStatusReason: "
                + notification.notification.status.subStatusReason.toString() + ", uri: " + notification.uri + "]");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.InstallNotification+XML";
        headers.add(field);

        int timeout = 20000;

        String body = xmlWrapper.serializeInstallNotification(notification);

        Log.v(LOG_TAG,
                LOG_PREFIX + " Calling doPostRequest with uri: " + softwareManagementUri
                        + (notification.uri.startsWith("/") ? notification.uri : "/" + notification.uri) + " and body: "
                        + body);
        Response response = cloudConnection.doPostRequest(softwareManagementUri + notification.uri, headers, body,
                timeout); // TODO: retrieve uri from software?

        return response.httpResponse;
    }

    private InstallNotificationResponse fetchInstallNotification(String installationOrderId, String uri) {
        Log.v(LOG_TAG, LOG_PREFIX + " fetchInstallNotification: [installationOrderId: " + installationOrderId + "]");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.InstallNotification+XML";
        headers.add(field);

        int timeout = 20000;

        InstallNotificationResponse notificationResponse = new InstallNotificationResponse();

        try {
            // Send request
            Log.v(LOG_TAG, LOG_PREFIX + " Calling doGetRequest with uri:" + (uri.startsWith("/") ? uri : "/" + uri)
                    + "and query: ?installation_order_id=" + installationOrderId);
            Response response = cloudConnection.doGetRequest(softwareManagementUri
                    + (uri.startsWith("/") ? uri : "/" + uri) + "?installation_order_id=" + installationOrderId,
                    headers, timeout);

            if (!HandleHttpResponseCode(response.httpResponse)) {
                Log.w(LOG_TAG, LOG_PREFIX + " Http Response Code: " + response.httpResponse
                        + ".\nSomething went bananas with the request. And it is not handled properly :'(");
            }

            // Parse response
            byte[] bytesdata = new byte[response.responseData.size()];
            for (int i = 0; i < bytesdata.length; i++) {
                bytesdata[i] = response.responseData.get(i);
            }

            Log.v(LOG_TAG, LOG_PREFIX + " Response: " + new String(bytesdata));

            InputStream stream = new ByteArrayInputStream(bytesdata);

            notificationResponse.notification = InstallNotificationParser.ParseInstallNotification(stream);
            notificationResponse.code = response.httpResponse;

        } catch (XmlPullParserException ex) {
            // Something went bananas with the parsing.. What do?
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Cannot parse response data: XmlPullParserException [" + ex.getMessage() + "]");
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, LOG_PREFIX + " Cannot read input data stream: IOException [" + ex.getMessage() + "]");
        }

        return notificationResponse;
    }

    private int postInstallationReport(InstallationReport report) {
        Log.v(LOG_TAG, LOG_PREFIX + " report: [installationOrderId: " + report.installationOrderId + ", reportReason: "
                + report.reportReason.toString() + "]");

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.InstallationReport+XML";
        headers.add(field);

        int timeout = 20000;

        String body = xmlWrapper.serializeInstallationReport(report);

        Log.v(LOG_TAG, LOG_PREFIX + " Calling doPostRequest with uri: " + softwareManagementUri
                + (report.uri.startsWith("/") ? report.uri : "/" + report.uri) + " and body: " + body);
        Response response = cloudConnection.doPostRequest(
                softwareManagementUri + (report.uri.startsWith("/") ? report.uri : "/" + report.uri), headers, body,
                timeout); // TODO: retrieve uri from software?

        return response.httpResponse;
    }

    private boolean HandleHttpResponseCode(final int code) {

        return code == 200;
    }

    /**
     * Get available assignments with specified query parameters
     *
     * @param queryParams Query parameters
     * @param type        AssignmentType (possible values: UPDATE or ACCESSORY)
     * @param callback
     */
    public void GetSoftwareAssignment(Query query, AssignmentType type, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.SoftwareAssignmentList(-1, type, null);
        }

        SwListResponse<SoftwareAssignment> swrsp = FetchSoftwareAssignment(query, type);
        Log.d(LOG_TAG, LOG_PREFIX + " GetSoftwareAssignment: list size: " + swrsp.swlist.size());

        if (query.id.isEmpty() && query.installationOrderId.isEmpty()) {
            callback.SoftwareAssignmentList(swrsp.code, type, swrsp.swlist);
        } else {
            if (swrsp.swlist.size() > 1) {
                Log.w(LOG_TAG, LOG_PREFIX
                        + " Size of returned list is >1, which should not happen since request was called with query containing id. What to do?");
            } else {
                callback.SoftwareAssignment(swrsp.code, query, type, swrsp.swlist.get(0));
            }
        }
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     *
     * @param commissionElement commission element containing id of the assignment
     *                          to fetch
     * @param callback
     */
    @Override
    public void CommissionSoftwareAssignment(CommissionElement commissionElement,
            ISoftwareManagementApiCallback callback) throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.CommissionStatus(commissionElement.id, -1);
        }

        callback.CommissionStatus(commissionElement.id, CommissionSoftwareAssignment(commissionElement));
    }

    /**
     * Get Download Info for an installation order
     *
     * @param installationOrder installation order
     * @param callback
     */
    public void GetDownloadInfo(InstallationOrder installationOrder, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.DownloadInfo(-1, null);
        }

        DownloadInfoResponse downloadInfoResponse = FetchDownloadInfo(installationOrder);
        callback.DownloadInfo(downloadInfoResponse.code, downloadInfoResponse.downloadInfo);
    }

    /**
     * Get Download data
     *
     * @param downloadInfo Contains information of what to be downloaded
     * @param callback     Callback to be called when the status of the download
     *                     changes
     */
    public void GetDownloadData(DownloadInfo downloadInfo, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.DownloadData(-1, null);
            return;
        }

        // TODO: Verify that no download is in progress already, if so... call callback
        // function with some busy info...
        if (softwareManagementApiCallback != null) {
            Log.w(LOG_TAG, LOG_PREFIX + " Download already in progress, try later...");
            callback.DownloadData(-1, null);
            return;
        }

        Log.v(LOG_TAG,
                LOG_PREFIX + " Download requested, try to download " + downloadInfo.resourceUris.size() + " files");

        softwareManagementApiCallback = callback;
        currentDownloadInfo = downloadInfo;

        FetchDownloadData();
    }

    /**
     * Post InstallationReport
     *
     * @param installationReport Report to be posted
     * @param callback           Callback to be called when the status of the
     *                           download changes
     */
    public void PostInstallationReport(InstallationReport installationReport, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.InstallationReportStatus(-1, installationReport.installationOrderId);
            return;
        }

        callback.InstallationReportStatus(postInstallationReport(installationReport),
                installationReport.installationOrderId);
    }

    /**
     * Post InstallNotification
     *
     * @param notification InstallNotification to be posted
     * @param callback     Callback to be called
     */
    public void PostInstallNotification(InstallNotification notification, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        Log.d(LOG_TAG, LOG_PREFIX + " PostInstall");
        if (!softwareManagementAvailable) {
            callback.InstallNotificationStatus(-1, notification.installationOrderId);
            return;
        }

        callback.InstallNotificationStatus(postInstallNotification(notification), notification.installationOrderId);
    }

    /**
     * Get InstallNotification
     *
     * @param installationOrderId Installation order id
     * @param uri                 Install notification uri
     * @param callback            Callback to be called
     */
    public void GetInstallNotification(String installationOrderId, String uri, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (!softwareManagementAvailable) {
            callback.InstallNotification(-1, null);
            return;
        }

        InstallNotificationResponse notificationResponse = fetchInstallNotification(installationOrderId, uri);
        callback.InstallNotification(notificationResponse.code, notificationResponse.notification);
    }
}