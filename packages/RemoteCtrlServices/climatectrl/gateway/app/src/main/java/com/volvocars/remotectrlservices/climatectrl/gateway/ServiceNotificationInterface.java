/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

public interface ServiceNotificationInterface {
    void notifyServiceConnected();
    void notifyServiceDisconnected();
}
