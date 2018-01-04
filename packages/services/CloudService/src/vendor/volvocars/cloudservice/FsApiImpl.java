package vendor.volvocars.cloudservice;

import android.util.Log;
import android.os.RemoteException;
import vendor.volvocars.hardware.http.V1_0.*;
import java.util.ArrayList;
import java.io.ByteArrayInputStream;
import java.io.InputStream;

/**
 * Implementation of Foundation service API.
 */
public class FsApiImpl{
    private IHttpRequest server;
    private static final String LOG_TAG = "CloudService.FsApi";
    public FsApiImpl(IHttpRequest server){
        this.server = server;
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
            field.value = "Accept: application/volvo.cloud.Features+XML";
            field.name = "features";
            ArrayList<HttpHeaderField> headers = new ArrayList<HttpHeaderField>();
            headers.add(field);
            String uri = "features-1";
            boolean useHttps = true;
            int timeout = 15;
            //DoRequest
            try{
                Response response = server.doGetRequest(uri,headers, useHttps, timeout);
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