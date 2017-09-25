package com.volvocars.interfacetest;

import android.net.Network;
import android.os.Bundle;
import android.util.Log;
import android.net.ConnectivityManager;
import android.widget.TextView;
import android.content.Context;
import android.content.Intent;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.app.Activity;

public class MainActivity extends Activity
{
  private String TAG;
  private TextView textField;
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
        TAG = "ihuInterfaceTest:";
        Log.d("ihuInterfaceTest:", "No caller key found!");
      }
    }
    else
    {
      TAG = "ihuInterfaceTest:";
      Log.d("ihuInterfaceTest:", "No caller key found! - Empty Intent");
    }
    doTest();
  }

  private void doTest()
  {
    String a = checkNetworkInterfaces();
    String b = checkConnectivityManagerInterfaces();
  }

  private String checkNetworkInterfaces()
  {
    int ifaceCounter = 0;
    String hmiOutput = "checkNetworkInterfaces: "
        + "\n";
    try
    {
      Enumeration<NetworkInterface> ifaces = NetworkInterface.getNetworkInterfaces();
      while (ifaces.hasMoreElements())
      {
        NetworkInterface iface = ifaces.nextElement();
        // Output to both logcat and Textview
        Log.i(TAG, iface.toString());
        hmiOutput += iface.toString() + "\n";
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

  private String checkConnectivityManagerInterfaces()
  {
    Network[] networks = connMngr.getAllNetworks();
    Log.i(TAG, "#Networks found: " + Integer.toString(networks.length));
    String hmiOutput = "checkConnectivityManagerInterfaces: "
        + "\n";
    for (int i = 0; i < networks.length; i++)
    {
      String iface = connMngr.getNetworkInfo(networks[i]).getTypeName() + "   |   "
          + connMngr.getNetworkInfo(networks[i]).getExtraInfo() + "\n";
      Log.i(TAG, iface);
      hmiOutput += iface;
    }
    return hmiOutput;
  }
}
