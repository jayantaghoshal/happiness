/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.generalsettingsgateway;

import com.volvocars.remotectrlservices.gateway.RemoteGateway;

public class RemoteGeneralSettingsGateway extends RemoteGateway {
    private static final String LOG_TAG = "RemoteCtrl.GeneralService.Gateway";
    private static final int DEATH_RECIPIENT_COOKIE = 1994;
    private static final String HAL_SERVICE_NAME = "RemoteCtrl_GeneralSettingsCtrl";

    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_INTERFACE_PACKAGE =
            "com.volvocars.remotectrlservice.generalsettingsservice.service";

    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_BIND_INTENT =
            ".RemoteGeneralSettingsService.BIND";
    private static final String REMOTE_CTRL_PROP_SERVICE_CLASS = "RemoteGeneralSettingsService";
    private static final String REMOTE_GENERAL_SETTINGS_SERVICE_RESPONSE_BIND_INTENT =
            ".RemoteGeneralSettingsGateway.BIND";
    // HAL -> Gateway -> Service communication

    @Override
    public int getDeathRecipientCookie() {
        return DEATH_RECIPIENT_COOKIE;
    }

    @Override
    public String getRemoteCtrlPropServiceBindIntent() {
        return REMOTE_GENERAL_SETTINGS_SERVICE_BIND_INTENT;
    }

    @Override
    public String getRemoteCtrlPropServicePackage() {
        return REMOTE_GENERAL_SETTINGS_SERVICE_INTERFACE_PACKAGE;
    }

    @Override
    public String getRemoteCtrlPropServiceClass() {
        return REMOTE_GENERAL_SETTINGS_SERVICE_INTERFACE_PACKAGE + "."
                + REMOTE_CTRL_PROP_SERVICE_CLASS;
    }

    @Override
    public String getRemoteCtrlPropResponseServiceOnBindIntent() {
        return REMOTE_GENERAL_SETTINGS_SERVICE_RESPONSE_BIND_INTENT;
    }

    @Override
    public String getHalServiceName() {
        return HAL_SERVICE_NAME;
    }
}
