/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.usb2ethernet;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import java.lang.String;

public class Usb2EthernetActivity extends Activity{
    private ConnectivityManager connMngr;
    private static final String TAG = "Usb2ethernetActivityApp";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        connMngr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
    }

    public int getNetworkType(){
        NetworkInfo net_info = connMngr.getActiveNetworkInfo();
        //getActiveNetworkInfo can return NULL if no active network exist.
        if(net_info == null) {
            return -1;
        }
        else {
            return net_info.getType();
        }
    }
}