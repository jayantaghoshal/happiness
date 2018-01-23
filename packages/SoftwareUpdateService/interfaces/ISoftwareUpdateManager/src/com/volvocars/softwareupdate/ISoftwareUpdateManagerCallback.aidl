package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.SoftwareAssignment;

/**
 * A callback interface to enable non-blocking request calls
 */
interface ISoftwareUpdateManagerCallback {
    void UpdateState(in int state);

    void UpdateSoftwareAssignmentList(in List<SoftwareAssignment> software_list);

    void UpdateSoftwareState(in String uuid, in int state);

    void ProvideErrorMessage(in int code, in String message);
}
