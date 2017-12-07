/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.usb2ethernet;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.net.ConnectivityManager;
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
        return connMngr.getActiveNetworkInfo().getType();
    }
}