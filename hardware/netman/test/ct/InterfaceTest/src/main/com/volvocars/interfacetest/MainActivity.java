package com.volvocars.networkinterfacetest;

import android.net.Network;
import android.os.Bundle;
import android.util.Log;
import android.net.ConnectivityManager;
import android.content.Context;
import android.content.Intent;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Activity;

public class MainActivity extends Activity
{
    private String TAG;
    private ConnectivityManager connMngr;

    @Override protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        connMngr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        if (bundle != null)
        {
            if (bundle.containsKey("caller"))
            {
                TAG = bundle.getString("caller");
            }
            else
            {
                TAG = "networkInterfaceTest:";
                Log.d("networkInterfaceTest:", "No caller key found!");
            }
        }
        else
        {
            TAG = "networkInterfaceTest:";
            Log.d("networkInterfaceTest:", "No caller key found! - Empty Intent");
        }
    }

    public ArrayList<String> checkNetworkInterfaces()
    {
        int ifaceCounter = 0;
        ArrayList<String> hmiOutput = new ArrayList<String>();
        try
        {
            Enumeration<NetworkInterface> ifaces = NetworkInterface.getNetworkInterfaces();
            Pattern pattern = Pattern.compile("\\((.*?)\\)");
            while (ifaces.hasMoreElements())
            {
                NetworkInterface iface = ifaces.nextElement();
                // Output to both logcat and Textview
                Log.i(TAG, iface.toString());
                Matcher matcher = pattern.matcher(iface.toString());
                if (matcher.find()) {
                    hmiOutput.add(matcher.group(1));
                }
                ifaceCounter++;
            }
        }
        catch (SocketException e)
        {
            Log.e(TAG, "Failed to fetch list of network interfaces (App permissions set correctly?)");
        }
        Log.i(TAG, "#Interfaces found:" + Integer.toString(ifaceCounter));
        return hmiOutput;
    }
}
