package com.volvocars.cloudservice;

import com.volvocars.cloudservice.IFoundationServicesApiCallback;

/**
 * Interface for Foundation service API.
 */
interface IFoundationServicesApi{
    /**
     * Check if feature is available
     * Returns false if Feature list isn't available (connection to cloud is missing), try again...
     */
    boolean isFeatureAvailable(in String feature, in IFoundationServicesApiCallback callback);
}
