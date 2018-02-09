package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareUpdateManagerCallback {
    /**
     * Notify client about a new state
     */
    void UpdateState(in int state);

    /**
     * Notify client about new Software Assignment List
     */
    void UpdateSoftwareAssignmentList(in List<SoftwareAssignment> software_list);

    /**
     * Notify client about a state change of a specific assignment
     */
    void UpdateSoftwareState(in String uuid, in int state);

    /**
    * Notify client about new Pending Installations (Installation Order List)
    */
    void UpdatePendingInstallations(in List<InstallationOrder> installation_order_list);

    /**
     * Notify client of Error
     */
    void ProvideErrorMessage(in int code, in String message);
}
