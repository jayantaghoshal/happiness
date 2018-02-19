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
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.SoftwareManagement+XML";

        headers.add(field);

        try {
            // Send request
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
            Log.e(LOG_TAG, "Something went bananas with the parsing: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: " + ex.getMessage());
        }
    }

    private SwListResponse<SoftwareAssignment> FetchSoftwareAssignmentsList() {
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
        Response response = cloudConnection.doPostRequest(softwareManagementUri + "/commission", headers, body,
                timeout);

        Log.d(LOG_TAG, " " + response.httpResponse);

        return response.httpResponse;

    }

    private String GetSystemLanguage() {
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
        // Build request
        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();
        Log.v(LOG_TAG, "FetchPendingInstallations");
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
            Log.e(LOG_TAG, "Something went bananas with the parsing: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: " + ex.getMessage());
        }

        return swrsp;
    }

    private DownloadInfoResponse FetchDownloadInfo(String uuid) {

        ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();

        HttpHeaderField field = new HttpHeaderField();
        field.name = "Accept";
        field.value = "application/volvo.cloud.software.Downloads+XML";
        headers.add(field);

        int timeout = 20000;

        DownloadInfoResponse downloadInfoResponse = new DownloadInfoResponse();

        try {
            // Send request
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
            Log.e(LOG_TAG, "Something went bananas with the parsing: " + ex.getMessage());
        } catch (IOException ex) {
            // Something went bananas with the streams.. What do?
            Log.e(LOG_TAG, "Something went bananas with the streams: " + ex.getMessage());
        }
        return downloadInfoResponse;
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
    public void GetDownloadData(DownloadInfo downloadInfo, ISoftwareManagementApiCallback callback) /* throws RemonteException */ {
        // TODO
    }
}