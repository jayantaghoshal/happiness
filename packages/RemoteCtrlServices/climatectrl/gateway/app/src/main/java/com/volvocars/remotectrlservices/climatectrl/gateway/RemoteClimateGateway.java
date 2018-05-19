/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import com.volvocars.remotectrlservices.gateway.RemoteGateway;

public class RemoteClimateGateway extends RemoteGateway {

    // TODO: Get package, class name programmatically instead of hardcoding /Philip Werner (2018-05-24)
    private static final String REMOTE_CTRL_PROP_SERVICE_PACKAGE = "com.volvocars.remotectrlservices.climatectrl.service";

    private static final String REMOTE_CTRL_PROP_SERVICE_CLASS = "RemoteClimateService";

    // TODO: Get bind intent programmatically instead of hardcoding /Philip Werner (2018-05-24)
    private static final String REMOTE_CTRL_PROP_SERVICE_BIND_INTENT = ".RemoteClimateService.BIND";

    private static final String REMOTE_CTRL_PROP_RESPONSE_SERVICE_ON_BIND_INTENT = ".RemoteClimateResponseService.BIND";

    private static final int DEATH_RECIPIENT_COOKIE = 2091;

    private static final String HAL_SERVICE_NAME = "RemoteCtrl_ClimateCtrl";

    @Override
    public String getRemoteCtrlPropServicePackage() {
        return REMOTE_CTRL_PROP_SERVICE_PACKAGE;
    }

    @Override
    public String getRemoteCtrlPropServiceClass() {
        return REMOTE_CTRL_PROP_SERVICE_PACKAGE + "." + REMOTE_CTRL_PROP_SERVICE_CLASS;
    }

    @Override
    public String getRemoteCtrlPropServiceBindIntent() {
        return REMOTE_CTRL_PROP_SERVICE_BIND_INTENT;
    }

    @Override
    public String getRemoteCtrlPropResponseServiceOnBindIntent() {
        return REMOTE_CTRL_PROP_RESPONSE_SERVICE_ON_BIND_INTENT;
    }

    @Override
    public int getDeathRecipientCookie() {
        return DEATH_RECIPIENT_COOKIE;
    }

    @Override
    public String getHalServiceName() {
        return HAL_SERVICE_NAME;
    }
}
