package vendor.volvocars.cloudservice;

import vendor.volvocars.cloudservice.SoftwareAssignment;

/**
 * A callback interface to enable non-blocking request calls
 */
interface ISoftwareManagementAPICallback{
    /**
     * Return result for CommissionSoftwareAssignment when received.
     * @param code The HTTP code of the response
     */
    void CommissionStatus(in int code);

    /**
     * Return result for GetSoftwareAssignmentList when received.
     * @param code          The HTTP code of the response
     * @param software_list The list of SoftwareAssignments
     */
    void SoftwareAssignmentList(in int code, in List<SoftwareAssignment> software_list);
}
