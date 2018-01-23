package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.SoftwareAssignment;

import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;

interface ISoftwareUpdateManager {
    void GetState(in ISoftwareUpdateManagerCallback callback);

    void GetSoftwareAssignments(in ISoftwareUpdateManagerCallback callback);

    void CommissionAssignment(in String uuid);
}