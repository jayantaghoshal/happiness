/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;
import java.util.ArrayList;
/**
*
*/
public class SoftwareManagementURIs{
    public String available_software_assignments = "";
    public String downloads = "";

    public SoftwareManagementURIs(String available_software_assignments, String downloads){
        this.available_software_assignments = available_software_assignments;
        this.downloads = downloads;
    }

}