package com.volvocars.cloudservice;

import com.volvocars.cloudservice.Feature;

/**
 * Interface for Foundation service API.
 */
oneway interface IFoundationServicesApiCallback{
    /**
     * Check if feature is available
     */
    void featureAvailableResponse(in Feature feature);
}
