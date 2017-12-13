package vendor.volvocars.cloudservice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.Intent;
import android.os.Bundle;
import vendor.volvocars.hardware.http.V1_0.*;
import java.util.ArrayList;
/**
* CloudService is the main service for communcating with clouddeamon.
*/
public class CloudService extends Service {
    private static final String LOG_TAG = "CloudService";
    private IfsApi.Stub fsapiBinder = null;
    private IHttpRequest server = null;
    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();
        try{
            server = IHttpRequest.getService();
            fsapiBinder = new FsApiImpl(server).binder;
        }
        catch(Exception ex){
            Log.e(LOG_TAG, ex.getMessage());
        }
    }
    /**
    * When a client connects to the service,
    * the intent action is read to identify which binder interface to return
    */
    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        String action = intent.getAction();
        if(action.equals("FsApi")){
            Log.v(LOG_TAG, "fsapiBinder returned");
            return fsapiBinder;
        }
        else{//handling for when couldnt find mathing binder?
            return null;
        }
    }
}