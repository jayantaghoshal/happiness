package com.volvocars.softwareupdate;

import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;
import com.volvocars.softwareupdate.ISoftwareUpdateSettingsCallback;

import com.volvocars.cloudservice.AssignmentType;
import com.volvocars.cloudservice.Query;

oneway interface ISoftwareUpdateManager {
    /**
     * Register a callback for state updates regarding SW updates
     */
    void RegisterSwUpdClient(in ISoftwareUpdateManagerCallback callback);

    /**
     * Unregister a callback for state updates regarding SW updates
     */
    void UnregisterSwUpdClient(in ISoftwareUpdateManagerCallback callback);

    /**
     * Register a callback for state updates regarding settings
     */
    void RegisterSettingsClient(in ISoftwareUpdateSettingsCallback callback);

    /**
     * Unregister a callback for state updates regarding settings
     */
    void UnregisterSettingsClient(in ISoftwareUpdateSettingsCallback callback);

    /**
    * Request the list of available Software Assignments with specified query parameters
    */
    void GetSoftwareAssignment(in Query query, in AssignmentType type);

    /**
     * Request to commission a SoftwareAssignment
     */
    void CommissionAssignment(in String uuid);

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

    /**
    * Set (OTA) setting
    * @param setting Setting to be set
    */
     void SetSetting(in String key, in boolean value);

}
