package com.volvocars.cloudservice;

import com.volvocars.cloudservice.AssignmentType;
import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.InstallationReport;
import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.Query;
import com.volvocars.cloudservice.CommissionElement;

/**
 * An interface to access all the Software Management features.
 */
oneway interface ISoftwareManagementApi {

    /**
     * Get available assignments with specified query parameters
     * @param query Query
     * @param type AssignmentType (possible values: UPDATE or ACCESSORY)
     * @param callback
     */
    void GetSoftwareAssignment(in Query query, in AssignmentType type, in ISoftwareManagementApiCallback callback);

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param commissionElement commission element containing id of the assignment to fetch
     * @param callback
     */
    void CommissionSoftwareAssignment(in CommissionElement commissionElement, in ISoftwareManagementApiCallback callback);

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

    /**
    * Get InstallNotification
    * @param installationOrderId Installation order id
    * @param callback            Callback to be called
    */
    void GetInstallNotification(in String installationOrderId, in ISoftwareManagementApiCallback callback);
}
