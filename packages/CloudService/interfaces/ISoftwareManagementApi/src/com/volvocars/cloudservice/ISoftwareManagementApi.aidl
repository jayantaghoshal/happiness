package com.volvocars.cloudservice;

import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.SoftwareAssignment;

/**
 * An interface to access all the Software Management features.
 */
interface ISoftwareManagementApi {
    /**
     * Get a the list of available assignments
     * @param callback
     */
    void GetSoftwareAssigmentList(in ISoftwareManagementApiCallback callback);

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     * @param callback
     */
     void CommissionSoftwareAssignment(in String uuid, in ISoftwareManagementApiCallback callback);
}
