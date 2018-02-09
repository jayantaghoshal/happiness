/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;
import java.util.ArrayList;
/**
*
*/
public class SoftwareManagementURIs{
    public String available_software_assignments = "";
    public String downloads = "";
    public String pending_installations = "";

    public SoftwareManagementURIs(String available_software_assignments, String downloads, String pending_installations){
        this.available_software_assignments = available_software_assignments;
        this.downloads = downloads;
        this.pending_installations = pending_installations;
    }

}