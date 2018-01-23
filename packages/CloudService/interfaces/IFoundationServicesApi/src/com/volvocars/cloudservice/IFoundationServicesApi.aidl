package com.volvocars.cloudservice;
/**
 * Interface for Foundation service API.
 */
interface IFoundationServicesApi{
    /**
     * Check if feature is available
     */
    boolean IsFeatureAvailable(String feature);
}
