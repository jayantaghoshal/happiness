package vendor.volvocars.cloudservice;

import android.util.Log;
import android.app.Service;
import android.os.IBinder;
import android.content.*;
import android.os.RemoteException;
/**
* Library/wrapper class that is used to communicate with FSAPI (Foundation Services API)
*/
public class FsApi implements ServiceConnection{
    private static final String LOG_TAG = "FsApi";
    private static final String PACKAGENAME = "vendor.volvocars.cloudservice";
    private static final String PACKAGENAME_SERVICENAME = "vendor.volvocars.cloudservice.CloudService";
    private Context mContext = null;
    private IfsApi mService = null;
    private boolean mBound = false;
    private FsApiConnectionCallback mfsApiConnectionCallback= null;
    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        mService = IfsApi.Stub.asInterface(service);
        mBound = true;
        mfsApiConnectionCallback.onServiceConnected();
    }
    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        mBound = false;
        mfsApiConnectionCallback.onServiceDisconnected();
    }
    /**
    * Constuctor FsApi
    * @param  context  application context, used to bind service to.
    * @param  fsApiConnectionCallback fsApiConnectionCallbackInterface
    */
    public FsApi(Context context, FsApiConnectionCallback fsApiConnectionCallback) {
        mContext = context;
        mfsApiConnectionCallback = fsApiConnectionCallback;
        connect();
    }
    /**
    * Used to bind to service, can be used by client if service unbinds unexpectedly
    */
    public void connect(){
        Intent i = new Intent();
        i.setAction("FsApi");
        i.setComponent(new ComponentName(PACKAGENAME,PACKAGENAME_SERVICENAME));
        mContext.bindService(i, this, Context.BIND_AUTO_CREATE);
    }
    /**
    * Checks if feature is available in the foundation service api.
    * @param feature wanted feature
    * @return boolean indicating if this feature exist or not.
    */
    public boolean IsFeatureAvailable(String feature) throws RemoteException {
        if(mService!=null && mBound)
            return mService.IsFeatureAvailable(feature);
        else
            return false;
    }
}