package vendor.volvocars.cloudservice;
/**
 * Interface for Foundation service API.
 */
interface IfsApi{
    /**
     * Check if feature is available
     */
    boolean IsFeatureAvailable(String feature);
}
