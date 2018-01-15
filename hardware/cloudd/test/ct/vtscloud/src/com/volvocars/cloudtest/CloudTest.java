/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudtest;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;
import java.net.HttpURLConnection;
import java.net.URL;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.BufferedInputStream;
import java.io.OutputStreamWriter;
import java.lang.StringBuilder;
import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;
import org.junit.Before;
import java.lang.String;
import android.util.Log;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class CloudTest extends ActivityTestRule<MainActivity>{
    private static final String LOG_TAG = "CloudTest.Class";
    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule<>(MainActivity.class);

    public CloudTest(){
        super(MainActivity.class);
    }
    @Before
    public void initializeTest() {
        StringBuilder StringbuilderFeat = new StringBuilder();
        StringbuilderFeat.append("{");
        StringbuilderFeat.append("\"response\": \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\"?><features xmlns=\\\"http://schemas.volvocars.biz/conncar/foundation_services/features\\\" xmlns:ds=\\\"http://www.w3.org/2000/09/xmldsig#\\\"><this>/features-1/</this><feature><name>Send to Car</name><enabled>true</enabled><visible>true</visible><uri>/sendtocar-1</uri><tags><tag>ExternalNavi</tag><tag>navigation</tag></tags></feature><feature><name>eula</name><enabled>true</enabled><visible>false</visible><uri>/eula-2</uri><tags><tag>media</tag><tag>radio</tag></tags></feature><feature><name>carsharing</name><enabled>true</enabled><visible>false</visible><uri>/carsharing-1</uri></feature><feature><name>mca.interaktion</name><enabled>true</enabled><visible>true</visible><uri>/byran-1</uri></feature><feature><name>cmdjsp</name><enabled>true</enabled><visible>false</visible><uri>/cmdjsp-1</uri></feature><feature><name>TuneIn Classic</name><enabled>true</enabled><visible>true</visible><uri>/tuneinclassic-1</uri><tags><tag>media</tag></tags></feature><feature><name>Route_prediction</name><enabled>true</enabled><visible>false</visible><uri>/routeprediction-2</uri></feature><feature><name>baidutest</name><enabled>true</enabled><visible>true</visible><uri>/baidutest-1</uri><tags><tag>navigation</tag></tags></feature><feature><name>Radio Play</name><enabled>true</enabled><visible>true</visible><uri>/radioplay-1</uri><tags><tag>radio</tag></tags></feature><feature><name>fsapitester</name><enabled>true</enabled><visible>false</visible><uri>/fsapitester-1</uri><description>For testing purposes only</description></feature><feature><name>fdtp</name><enabled>true</enabled><visible>true</visible><uri>/theftprotection-1</uri><description>Rest interface for Function Discovery of Theft Protection Services</description></feature><feature><name>WikiLocations</name><enabled>true</enabled><visible>true</visible><uri>/wikipedia-1</uri><tags><tag>ExternalNavi</tag><tag>navigation</tag></tags></feature><feature><name>Places</name><enabled>true</enabled><visible>false</visible><uri>/places-1</uri></feature><feature><name>Pandora</name><enabled>true</enabled><visible>true</visible><uri>/pandora-1</uri><tags><tag>radio</tag></tags></feature><feature><name>Proxy</name><enabled>true</enabled><visible>false</visible><uri>/proxy-1</uri><icon>/img/icon.png</icon><description>Description of Proxy</description></feature><feature><name>CarSettings</name><enabled>true</enabled><visible>false</visible><uri>/carsettings-1</uri><icon>/img/icon.png</icon><description>Description of CarSettings</description></feature><feature><name>QRdio</name><enabled>true</enabled><visible>true</visible><uri>/qrdio-1</uri></feature><feature><name>ExternalDiagnostics</name><enabled>true</enabled><visible>false</visible><uri>/externaldiagnostics-1</uri><icon>/img/icon.png</icon><description>Description of External Diagnostics</description></feature></features>\"");
        StringbuilderFeat.append(",");
        StringbuilderFeat.append("\"request\": {");
        StringbuilderFeat.append("\"type\": \"GET\",");
        StringbuilderFeat.append("\"path\": \"/cloud/features-1\",");
        StringbuilderFeat.append("\"version\":\"HTTP/1.0\",");
        StringbuilderFeat.append("\"headers\": [{ \"Accept\": \"application/volvo.cloud.Features+XML\"}]");
        StringbuilderFeat.append("}");
        StringbuilderFeat.append("}");
        Log.d(LOG_TAG,StringbuilderFeat.toString());
        excutePost("http://127.0.0.1:8081/cloud/features-1", StringbuilderFeat.toString());
        try{
            Thread.sleep(2000);
        }
        catch(Exception ex){
            Log.d(LOG_TAG,ex.getMessage());
        }
    }

    public static void excutePost(String targetURL, String urlParameters)
    {
        URL url;
        HttpURLConnection connection = null;
        StringBuffer sb = new StringBuffer();
        InputStream is = null;
        try {
            //Create connection
            String result = "";
            url = new URL(targetURL);
            connection = (HttpURLConnection)url.openConnection();
            connection.setRequestMethod("PUT");
            connection.setUseCaches(false);
            connection.setDoOutput(true);
            connection.setDoInput(true);

            OutputStreamWriter out = new OutputStreamWriter(connection.getOutputStream());
            out.write(urlParameters);
            out.flush();
            out.close();

            //Get Response
            is = new BufferedInputStream(connection.getInputStream());
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String inputLine = "";
            while ((inputLine = br.readLine()) != null) {
                sb.append(inputLine);
            }
            result = sb.toString();
            Log.d(LOG_TAG,"result: "+result);
        }
        catch (Exception e) {
            Log.d(LOG_TAG, "Error reading InputStream "); //python server, dont response properly?
            Log.d(LOG_TAG,e.getMessage());
            e.printStackTrace();
        }
        finally {
            if(connection != null) {
                connection.disconnect();
            }
        }
    }
    @Test
    public void IsFeatureAvailableCarsharing() throws Exception {
        mActivityRule.getActivity().IsFeatureAvailableCarsharing();
    }
    @Test
    public void IsFeatureAvailableOta2() throws Exception {
        mActivityRule.getActivity().IsFeatureAvailableOta2();
    }
    @Test
    public void IsServiceConnected() throws Exception {
        mActivityRule.getActivity().IsServiceConnected();
    }
}