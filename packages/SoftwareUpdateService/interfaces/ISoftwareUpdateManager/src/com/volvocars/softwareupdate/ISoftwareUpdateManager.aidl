package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;

oneway interface ISoftwareUpdateManager {
    /**
     * Register a callback for state updates
     */
    void GetState(in ISoftwareUpdateManagerCallback callback);

    /**
     * Request the list of available Software Assignments.
     */
    void GetSoftwareAssignments(in ISoftwareUpdateManagerCallback callback);

    /**
     * Request to commission a SoftwareAssignment
     */
    void CommissionAssignment(in ISoftwareUpdateManagerCallback callback, in String uuid);

    /**
    * Get a list of pending installations (installation orders)
    * @param callback
    */
    void GetPendingInstallations(in ISoftwareUpdateManagerCallback callback);

    /**
    * Get Download Info for an installation order
    * @param uuid installation order id
    * @param callback
    */
     void GetDownloadInfo(in String uuid, in ISoftwareUpdateManagerCallback callback);
}
