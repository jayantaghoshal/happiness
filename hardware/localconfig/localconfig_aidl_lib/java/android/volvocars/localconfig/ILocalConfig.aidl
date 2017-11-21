package android.volvocars.localconfig;

/**
 * Binder interface for {@link android.volvocars.localconfig.LocalConfig}.
 * Check {@link android.volvocars.localconfig.LocalConfig} APIs for expected behavior of each calls.
 *
 * @hide
 */
interface ILocalConfig {
    /**
     * Get the Integer value corresponds to inputted Local configuration key string
     */
    int getLocalConfigInteger(in String key) = 0;
    /**
     * Get the String value corresponds to inputted Local configuration key string
     */
    String getLocalConfigString(in String key) = 1;
     /**
     * Get the boolean value corresponds to inputted Local configuration key string
     */
    boolean getLocalConfigBoolean(in String key) = 2;
    /**
     * Get the double value corresponds to inputted Local configuration key string
     */
    double getLocalConfigDouble(in String key) = 3;
}
