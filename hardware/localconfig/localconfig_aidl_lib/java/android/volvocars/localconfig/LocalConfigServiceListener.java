/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package android.volvocars.localconfig;

import android.content.ComponentName;

/**
 * Listener for monitoring local config service connection status.
 */
public interface LocalConfigServiceListener {
    /** Local Config service connected */
    void onServiceConnected(ComponentName name);
    /** Local Config service disconnected */
    void onServiceDisconnected(ComponentName name);
}