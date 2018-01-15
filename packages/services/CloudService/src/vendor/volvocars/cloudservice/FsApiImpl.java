/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

import android.util.Log;
import android.os.RemoteException;
import vendor.volvocars.hardware.cloud.V1_0.*;
import java.util.ArrayList;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import android.os.SystemProperties;
/**
 * Implementation of Foundation service API.
 */
public class FsApiImpl{
    private ICloudConnection server;
    private static final String LOG_TAG = "CloudService.FsApi";
    private Boolean _use_https = true;
    public FsApiImpl(ICloudConnection server){
        this.server = server;
        _use_https = SystemProperties.getBoolean("service.cloudservice.use_https",true);
        Log.d(LOG_TAG,"service.cloudservice.use_https: " + _use_https);
    }

    public IfsApi.Stub binder = new IfsApi.Stub() {
        /**
        * Performs request against cloudd.
        * Parses the response using xmlparser.
        * Searches for feature in arraylist structure.
        * @param feature string indicating which feature wanted.
        * @return boolean indicating if the feature exist.
        */
        @Override
        public boolean IsFeatureAvailable (String feature) throws RemoteException {
            //Setup request fields
            HttpHeaderField field = new HttpHeaderField();
            field.name = "Accept";
            field.value = "application/volvo.cloud.Features+XML";
            ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();
            headers.add(field);
            String uri = "features-1";
            int timeout = 10000; //Millisec
            //DoRequest
            try{
                Response response = server.doGetRequest(uri,headers, _use_https, timeout);
                byte[] bytesdata =  new byte[response.responseData.size()];
                for(int i=0; i<bytesdata.length;i++){
                    bytesdata[i] = response.responseData.get(i);
                }
                InputStream stream = new ByteArrayInputStream(bytesdata);
                //ParseData
                ArrayList<Feature> features = XmlParser.parse(stream);
                for(Feature f : features){
                    if(f.Name.equals(feature))
                        return true;
                }
                return false;
            }
            catch(Exception ex){
                //Todo some proper error handling
                Log.e(LOG_TAG,"ex: " + ex.getMessage());
            }
            return false;
        }
    };
}