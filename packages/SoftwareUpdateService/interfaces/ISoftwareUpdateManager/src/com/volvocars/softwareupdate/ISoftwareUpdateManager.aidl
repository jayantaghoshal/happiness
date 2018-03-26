package com.volvocars.softwareupdate;

import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;

oneway interface ISoftwareUpdateManager {
    /**
     * Register a callback for state updates
     */
    void GetState(in ISoftwareUpdateManagerCallback callback);

    /**
     * Request the list of available Software Assignments.
     */
    void GetSoftwareAssignments();

    /**
     * Request to commission a SoftwareAssignment
     */
    void CommissionAssignment(in String uuid);

    /**
    * Get a list of pending installations (installation orders)
    */
    void GetPendingInstallations();

    /**
    * Get Download Info for an installation order
    * @param uuid installation order id
    */
     void GetDownloadInfo(in String uuid);

     /**
     * Get InstallNotification for an installation order
     * @param installionOrderId installationOrderId
     */
     void GetInstallNotification(in String installationOrderId);

     void ShowInstallationPopup(in String installationOrderId);

}
