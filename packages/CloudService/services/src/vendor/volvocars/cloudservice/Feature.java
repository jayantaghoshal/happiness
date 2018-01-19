/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;
import java.util.ArrayList;
/**
* Class that matches structure of Volvo FS Api Feature element.
*/
public class Feature {
    public String Name="";
    public Boolean Enabled=false;
    public Boolean Visible=false;
    public String Uri ="";
    public ArrayList<String> Tags = null;
    public Feature(String name,Boolean enabled, Boolean visible, String uri,ArrayList<String> tags){
        Name=name;
        Enabled=enabled;
        Visible=visible;
        Uri =uri;
        Tags =tags;
    }

}