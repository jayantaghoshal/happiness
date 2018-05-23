/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.mediactrl.service.player;

public interface MediaPlayerCallback {


    void PlaybackStatusChanged(int playbackStatus, int fctAvailabilityStatus);

}