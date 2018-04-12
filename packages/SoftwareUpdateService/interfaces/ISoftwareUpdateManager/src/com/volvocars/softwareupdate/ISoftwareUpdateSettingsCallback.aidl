package com.volvocars.softwareupdate;

import com.volvocars.softwareupdate.Setting;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareUpdateSettingsCallback {

    /**
    * Notify client about update of setting
    */
    void UpdateSettings(in List<Setting> settings);
}
