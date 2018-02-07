/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import java.util.ArrayList;

/**
* Class that matches structure of Volvo FS Api Feature element.
*/
public class Feature {
    public String name = "";
    public boolean enabled = false;
    public boolean visible = false;
    public String uri = "";
    public ArrayList<String> tags = null;

    public Feature(String name, boolean enabled, boolean visible, String uri, ArrayList<String> tags) {
        this.name = name;
        this.enabled = enabled;
        this.visible = visible;
        this.uri = uri;
        this.tags = tags;
    }
}