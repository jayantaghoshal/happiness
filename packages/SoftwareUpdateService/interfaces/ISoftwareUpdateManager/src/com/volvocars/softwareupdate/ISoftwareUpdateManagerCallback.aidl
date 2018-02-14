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
     * Notify client about a state change of a specific assignment
     */
    void UpdateSoftwareState(in String uuid, in int state);

    /**
     * Notify client of Error
     */
    void ProvideErrorMessage(in int code, in String message);
}
