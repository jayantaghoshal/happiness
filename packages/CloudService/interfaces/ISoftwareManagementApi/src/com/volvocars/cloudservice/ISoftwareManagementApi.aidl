package com.volvocars.cloudservice;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.InstallationReport;
import com.volvocars.cloudservice.InstallNotification;

/**
 * An interface to access all the Software Management features.
 */
oneway interface ISoftwareManagementApi {
    /**
     * Get a the list of available assignments
     * @param callback
     */
    void GetSoftwareAssigmentList(in ISoftwareManagementApiCallback callback);

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     * @param callback
     */
    void CommissionSoftwareAssignment(in String uuid, in ISoftwareManagementApiCallback callback);

    /**
     * Get a list of pending installations (installation orders)
     * @param callback
     */
    void GetPendingInstallations(in ISoftwareManagementApiCallback callback);

    /**
     * Get Download Info for an installation order
     * @param uuid installation order id
     * @param callback
     */
    void GetDownloadInfo(in String uuid, in ISoftwareManagementApiCallback callback);

    /**
     * Get Download data
     * @param downloadInfo Contains information of what to be downloaded
     * @param callback     Callback to be called when the status of the download changes
     */
    void GetDownloadData(in DownloadInfo downloadInfo, in ISoftwareManagementApiCallback callback);

    /**
     * Post InstallationReport
     * @param installationReport Report to be posted
     * @param callback           Callback to be called when the status of the download changes
     */
    void PostInstallationReport(in InstallationReport installationReport, in ISoftwareManagementApiCallback callback);

    /**
    * Post InstallNotification
    * @param notification InstallNotification to be posted
    * @param callback     Callback to be called
    */
    void PostInstallNotification(in InstallNotification notification, in ISoftwareManagementApiCallback callback);
}
