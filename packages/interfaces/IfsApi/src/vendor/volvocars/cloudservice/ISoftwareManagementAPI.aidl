package vendor.volvocars.cloudservice;

import vendor.volvocars.cloudservice.ISoftwareManagementAPICallback;
import vendor.volvocars.cloudservice.SoftwareAssignment;

/**
 * An interface to access all the Software Management features.
 */
interface ISoftwareManagementAPI{
    /**
     * Get a the list of available assignments
     * @param callback
     */
    void GetSoftwareAssigmentList(in ISoftwareManagementAPICallback callback);

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     * @param callback
     */
     void CommissionSoftwareAssignment(in String uuid, in ISoftwareManagementAPICallback callback);
}
