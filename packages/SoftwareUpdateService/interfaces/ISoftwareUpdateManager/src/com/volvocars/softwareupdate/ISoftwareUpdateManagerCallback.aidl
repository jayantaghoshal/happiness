package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;

import com.volvocars.softwareupdate.SoftwareInformation;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareUpdateManagerCallback {

    /**
     * Notify client about a new state
     */
    void UpdateState(in int state);

    /**
     * Notify Client About Update Software List.
     */
    void UpdateSoftwareList(in List<SoftwareInformation> software_list);

    /**
    * Notify client about update of software
    */
    void UpdateSoftware(in SoftwareInformation software);

    /**
     * Notify client of Error
     */
    void ProvideErrorMessage(in int code, in String message);
}
