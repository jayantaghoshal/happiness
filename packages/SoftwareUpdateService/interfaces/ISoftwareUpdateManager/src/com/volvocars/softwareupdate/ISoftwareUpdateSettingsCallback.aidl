package com.volvocars.softwareupdate;

/**
 * A callback interface to enable non-blocking request calls
 */
oneway interface ISoftwareUpdateSettingsCallback {

    /**
    * Notify client about update of setting
    */
    void UpdateSetting(in String key, in boolean value);
}
