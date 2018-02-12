package com.volvocars.cloudservice;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.InstallationOrder;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareManagementApiCallback {
    /**
     * Return result for CommissionSoftwareAssignment when received.
     * @param code The HTTP code of the response
     */
    void CommissionStatus(in int code);

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
    * @param uuids         The id for the installation order
    * @param download_info The download information of the installation order.
    */
    void DownloadInfo(in String uuid, in DownloadInfo download_info);
}
