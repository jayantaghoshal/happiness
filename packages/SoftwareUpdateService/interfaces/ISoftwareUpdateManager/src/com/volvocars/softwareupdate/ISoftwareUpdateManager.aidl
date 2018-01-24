package com.volvocars.softwareupdate;

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
    void CommissionAssignment(in String uuid);
}