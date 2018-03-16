package com.volvocars.cloudservice;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.InstallNotification;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareManagementApiCallback {
    /**
     * Return result for CommissionSoftwareAssignment when received.
     * @param uuid Id of commissioned software
     * @param code The HTTP code of the response
     */
    void CommissionStatus(in String uuid, in int code);

    /**
     * Return result for GetSoftwareAssignmentList when received.
     * @param code          The HTTP code of the response
     * @param software_list The list of SoftwareAssignments
     */
    void SoftwareAssignmentList(in int code, in List<SoftwareAssignment> software_list);

    /**
     * Return result for GetPendingInstallations when received.
     * @param code                    The HTTP code of the response
     * @param installation_order_list The list of InstallationOrders
     */
    void PendingInstallations(in int code, in List<InstallationOrder> installation_order_list);

    /**
     * Return the result for GetDownloadInfo
     * @param code          The HTTP cose of the response
     * @param download_info The download information of the installation order.
     */
    void DownloadInfo(in int code, in DownloadInfo download_info);

    /**
     * Return the result of GetDownloadData
     * @param code         The latest HTTP code when downloading
     * @param downloadInfo The latest information regaring the download
     */
    void DownloadData(in int code, in DownloadInfo downloadInfo);

    /**
     * Return the result of PostInstallationReport
     * @param code The HTTP code of the response
     * @param installationOrderId UUID of the posted Installation Report
     */
    void InstallationReportStatus(in int code, in String installationOrderId);

    /**
    * Return the result of PostInstallNotification
    * @param code The HTTP code of the response
    * @param installationOrderId installation order Id
    */
    void InstallNotificationStatus(in int code, in String installationOrderId);
}
