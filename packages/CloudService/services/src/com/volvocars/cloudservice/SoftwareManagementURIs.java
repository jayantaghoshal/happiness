/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;
import java.util.ArrayList;
/**
*
*/
public class SoftwareManagementURIs{
    public String available_updates = "";
    public String available_accessories = "";
    public String downloads = "";
    public String pending_installations = "";

    public SoftwareManagementURIs(String available_updates, String available_accessories, String downloads, String pending_installations){
        this.available_updates = available_updates;
        this.available_accessories = available_accessories;
        this.downloads = downloads;
        this.pending_installations = pending_installations;
    }

}