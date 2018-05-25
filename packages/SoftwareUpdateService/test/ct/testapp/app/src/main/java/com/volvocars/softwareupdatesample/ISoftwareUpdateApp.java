/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdatesample;

public interface ISoftwareUpdateApp {
    public abstract void commissionAssignment(String uuid);
    public abstract void getInstallNotification(String installationOrderId);
}