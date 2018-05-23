/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.mediactrl.gateway;

import com.volvocars.remotectrlservices.gateway.RemoteGateway;

public class RemoteMediaGateway extends RemoteGateway {

    private static final String LOG_TAG = "RemoteCtrl.MediaCtrl.Gateway";
    private static final int DEATH_RECIPIENT_COOKIE = 1993;
    private static final String HAL_SERVICE_NAME = "RemoteCtrl_MediaCtrl";
    private static final String REMOTE_MEDIA_SERVICE_INTERFACE_PACKAGE = "com.volvocars.remotectrlservices.mediactrl.service";
    private static final String REMOTE_CTRL_PROP_SERVICE_CLASS = "RemoteMediaStreamingService";
    private static final String REMOTE_MEDIA_SERVICE_BIND_INTENT = ".RemoteMediaStreamingService.BIND";
    private static final String REMOTE_MEDIA_SERVICE_RESPONSE_BIND_INTENT = ".RemoteMediaGateway.BIND";

    // HAL -> Gateway -> Service communication

    @Override
    public int getDeathRecipientCookie() {
        return DEATH_RECIPIENT_COOKIE;
    }

    @Override
    public String getRemoteCtrlPropServiceBindIntent() {
        return REMOTE_MEDIA_SERVICE_BIND_INTENT;
    }

    @Override
    public String getRemoteCtrlPropServicePackage() {
        return REMOTE_MEDIA_SERVICE_INTERFACE_PACKAGE;
    }

    @Override
    public String getRemoteCtrlPropServiceClass() {
        return REMOTE_MEDIA_SERVICE_INTERFACE_PACKAGE + "." + REMOTE_CTRL_PROP_SERVICE_CLASS;
    }

    @Override
    public String getRemoteCtrlPropResponseServiceOnBindIntent() {
        return REMOTE_MEDIA_SERVICE_RESPONSE_BIND_INTENT;
    }

    @Override
    public String getHalServiceName() {
        return HAL_SERVICE_NAME;
    }

}