package android.volvocars.localconfig;

import android.annotation.Nullable;
import android.annotation.SystemApi;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.RemoteException;
import android.os.UserHandle;
import android.util.Log;

import com.android.internal.annotations.GuardedBy;

/**
 *   Local Configuration API
 */
public final class LocalConfig {

    /** Service name for Local Config */
    /**
     * @hide
     */
    @SystemApi
    public static final String LCFG_SERVICE = "localconfig";
       /**
     * Permission necessary to access Local Config APIs.
     * @hide
     */
    @SystemApi
    public static final String PERMISSION_LCFG = "android.volvocars.permission.LOCAL_CONFIG";

    /** Interface name of Local Config Service*/
    /** @hide */
    public static final String LOCAL_CONFIG_SERVICE_INTERFACE_NAME = "android.volvocars.localconfig.ILocalConfig";

    /** Package name of Local Config Service*/
    private static final String LOCAL_CONFIG_SERVICE_PACKAGE = "com.volvocars.localconfig";

    /** Service class name of Local Config Service*/
    private static final String LOCAL_CONFIG_SERVICE_CLASS = "com.volvocars.localconfig.LcfService";

    /** RPC(Client <-> Service) -Service binding retry interval */
    private static final long LOCAL_CONFIG_SERVICE_BIND_RETRY_INTERVAL_MS = 500;

    /** RPC(Client <-> Service)-Service binding maximum retry count*/
    private static final long LOCAL_CONFIG_SERVICE_BIND_MAX_RETRY = 20;

    private final Context mContext;
    private final Looper mLooper;
    /** Intrinsic lock : Binder service instance*/
    @GuardedBy("this")
    private ILocalConfig mService;

    private static final int LCCFG_STATE_DISCONNECTED = 0;
    private static final int LCCFG_STATE_CONNECTING = 1;
    private static final int LCCFG_STATE_CONNECTED = 2;

    /** Intrinsic lock Annotation: Binder service connection state*/
    @GuardedBy("this")
    private int mConnectionState;
     /** Intrinsic lock Annotation: Binder service connection retry count*/
    @GuardedBy("this")
    private int mConnectionRetryCount;

    private final Runnable mConnectionRetryRunnable = new Runnable() {
        @Override
        public void run() {
            startLocalConfigService();
        }
    };

    private final Runnable mConnectionRetryFailedRunnable = new Runnable() {
        @Override
        public void run() {
            mServiceConnectionListener.onServiceDisconnected(new ComponentName(LOCAL_CONFIG_SERVICE_PACKAGE,
                    LOCAL_CONFIG_SERVICE_CLASS));
        }
    };

    private final ServiceConnection mServiceConnectionListener =
            new ServiceConnection () {
        public void onServiceConnected(ComponentName name, IBinder service) {
            synchronized (LocalConfig.this) {
                mService = ILocalConfig.Stub.asInterface(service);
                mConnectionState = LCCFG_STATE_CONNECTED;
            }
            mServiceConnectionListenerClient.onServiceConnected(name);
        }

        public void onServiceDisconnected(ComponentName name) {
            synchronized (LocalConfig.this) {
                mService = null;
                if (mConnectionState  == LCCFG_STATE_DISCONNECTED) {
                    return;
                }
                mConnectionState = LCCFG_STATE_DISCONNECTED;
            }
            // Explicitly Unbind the Local Config service
            disconnectlocalConfig();
            mServiceConnectionListenerClient.onServiceDisconnected(name);
        }
    };

    /** Local config Service listner instance for Client(System App) */
    private final LocalConfigServiceListener mServiceConnectionListenerClient;

    /** Handler for generic event dispatching. */
    private final Handler mEventHandler;
    private final Handler mMainThreadEventHandler;

    /**
     * A factory method that creates Local config instance
     * @param context
     * @param serviceConnectionListener listener for monitoring service connection.
     * @param looper Looper to dispatch all listeners. If null, it will use main UI thread. Note that
     *        service connection listener will be always in main thread regardless of this Looper.
     * @return Lcfg instance or returns {@code null} otherwise.
     */
    public static LocalConfig createLocalConfig(Context context, LocalConfigServiceListener serviceConnectionListener,
            @Nullable Looper looper) {
        try {
          return new LocalConfig(context, serviceConnectionListener, looper);
        } catch (IllegalArgumentException e) {
            Log.e(LocalConfigLog.TAG_LCLCFG, e.getMessage());

        }
        return null;
    }

    /**
     * A factory method that creates Local Config instance for all Local Config API access using main thread
     *
     * @see #createLocalConfig(Context, ServiceConnection, Looper)
     */
    public static LocalConfig createLocalConfig(Context context, LocalConfigServiceListener serviceConnectionListener) {
      return createLocalConfig(context, serviceConnectionListener, null);
    }

    private LocalConfig(Context context, LocalConfigServiceListener serviceConnectionListener,
            @Nullable Looper looper) {
        mContext = context;
        mServiceConnectionListenerClient = serviceConnectionListener;
        if (looper == null) {
            mLooper = Looper.getMainLooper();
        } else {
            mLooper = looper;
        }
        mEventHandler = new Handler(mLooper);
        if (mLooper == Looper.getMainLooper()) {
            mMainThreadEventHandler = mEventHandler;
        } else {
            mMainThreadEventHandler = new Handler(Looper.getMainLooper());
        }
    }

    /**
     * Lcfg constructor when ILocalConfig binder is already available.
     * @param context
     * @param service
     * @param looper
     *
     * @hide
     */
    public LocalConfig(Context context, ILocalConfig service, @Nullable Looper looper) {
        mContext = context;
        if (looper == null) {
            mLooper = Looper.getMainLooper();
        } else {
            mLooper = looper;
        }
        mEventHandler = new Handler(mLooper);
        if (mLooper == Looper.getMainLooper()) {
            mMainThreadEventHandler = mEventHandler;
        } else {
            mMainThreadEventHandler = new Handler(Looper.getMainLooper());
        }
        mService = service;
        mConnectionState = LCCFG_STATE_CONNECTED;
        mServiceConnectionListenerClient = null;
    }

    /**
     * Connect to Lcfg service. This can be called while it is disconnected.
     * @throws IllegalStateException If connection is still on-going from previous
     *  connect call or it is already connected
     */
    public void connectlocalConfig() throws IllegalStateException {
        synchronized (this) {
            if (mConnectionState != LCCFG_STATE_DISCONNECTED) {
                throw new IllegalStateException("Lcfg service already connected or connecting");
            }
            mConnectionState = LCCFG_STATE_CONNECTING;
            startLocalConfigService();
        }
    }

    /**
     * Disconnect from Local config service. This can be called while disconnected.
     */
    public void disconnectlocalConfig() {
        synchronized (this) {
            if (mConnectionState == LCCFG_STATE_DISCONNECTED) {
                return;
            }
            mEventHandler.removeCallbacks(mConnectionRetryRunnable);
            mMainThreadEventHandler.removeCallbacks(mConnectionRetryFailedRunnable);
            mConnectionRetryCount = 0;
            mService = null;
            mConnectionState = LCCFG_STATE_DISCONNECTED;
            mContext.unbindService(mServiceConnectionListener);
        }
    }

    /**
     * Tells if it is connected to the Local Config service or not. This will return false if it is still
     * connecting.
     * @return
     */
    public boolean isConnectedLocalConfig() {
        synchronized (this) {
            return mService != null;
        }
    }

    /**
     * Tells if this instance is already connecting to Local Config service or not.
     * @return
     */
    public boolean isConnectingLocalConfig() {
        synchronized (this) {
            return mConnectionState == LCCFG_STATE_CONNECTING;
        }
    }

    /**
     * Get the Integer value corresponds to inputted Local configuration key string
     * @param keystring Key string
     * @return int Integer value corresponds to Local configuration key.
     * @throws LcfgNotConnectedException
     */
    public int getLocalConfigInteger(String keystring) throws LcfgNotConnectedException, RuntimeException {

        int localconfigvalue = -1;
        ILocalConfig service = getILocalConfigOrThrow();
        synchronized (this) {
            try {

                localconfigvalue = service.getLocalConfigInteger(keystring);
                return localconfigvalue;

            } catch (RemoteException e) {
                 handleRemoteException(e);
            }
        }
        return localconfigvalue;
    }
    /**
     * Get the String value corresponds to inputted Local configuration key string
     * @param keystring Key string
     * @return String value corresponds to Local configuration key.
     * @throws LcfgNotConnectedException
     */
    public String getLocalConfigString(String keystring) throws LcfgNotConnectedException, RuntimeException {
        String localconfigvalue="";
        ILocalConfig service = getILocalConfigOrThrow();
        synchronized (this) {
            try {
                localconfigvalue = service.getLocalConfigString(keystring);
            } catch (RemoteException e) {
                 handleRemoteException(e);
            }
        }
        return localconfigvalue;
    }
    /**
     * Get the boolean value corresponds to inputted Local configuration key string
     * @param keystring Key string
     * @return boolean value corresponds to Local configuration key.
     * @throws LcfgNotConnectedException
     */
    public boolean getLocalConfigBoolean(String keystring) throws LcfgNotConnectedException, RuntimeException {
        boolean localconfigvalue = false;
        ILocalConfig service = getILocalConfigOrThrow();
        synchronized (this) {
            try {
                localconfigvalue = service.getLocalConfigBoolean(keystring);
            } catch (RemoteException e) {
                 handleRemoteException(e);
            }
        }
        return localconfigvalue;
    }
    /**
     * Get the boolean value corresponds to inputted Local configuration key string
     * @param keystring Key string
     * @return boolean value corresponds to Local configuration key.
     * @throws LcfgNotConnectedException
     */
    public double getLocalConfigDouble(String keystring) throws LcfgNotConnectedException, RuntimeException {
        double localconfigvalue = 0;
        ILocalConfig service = getILocalConfigOrThrow();
        synchronized (this) {
            try {
                 localconfigvalue = service.getLocalConfigDouble(keystring);
            } catch (RemoteException e) {
                 handleRemoteException(e);
            }
        }
        return localconfigvalue;
    }

    private void startLocalConfigService() {
        Intent intent = new Intent();
        intent.setPackage(LOCAL_CONFIG_SERVICE_PACKAGE);
        intent.setAction(LocalConfig.LOCAL_CONFIG_SERVICE_INTERFACE_NAME);
        boolean bound = mContext.bindServiceAsUser(intent, mServiceConnectionListener,
                Context.BIND_AUTO_CREATE, UserHandle.CURRENT_OR_SELF);
        if (!bound) {
            mConnectionRetryCount++;
            if (mConnectionRetryCount > LOCAL_CONFIG_SERVICE_BIND_MAX_RETRY) {
                Log.w(LocalConfigLog.TAG_LCLCFG, "cannot bind to local config service after max retry");
                mMainThreadEventHandler.post(mConnectionRetryFailedRunnable);
            } else {
                mEventHandler.postDelayed(mConnectionRetryRunnable,
                        LOCAL_CONFIG_SERVICE_BIND_RETRY_INTERVAL_MS);
            }
        } else {
            mConnectionRetryCount = 0;
        }
    }

    private synchronized ILocalConfig getILocalConfigOrThrow() throws IllegalStateException {
        if (mService == null) {
            throw new IllegalStateException("not connected");
        }
        return mService;
    }

    private void handleRemoteException(RemoteException e) {
        Log.w(LocalConfigLog.TAG_LCLCFG, "RemoteException", e);
        disconnectlocalConfig();
    }
}
