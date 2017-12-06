package com.volvocars.powerevent;

import android.app.Application;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import com.delphi.desip.IDesip;
import com.delphi.desip.IDesipListener;


public class MainApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        IBinder binder = ServiceManager.getService(IDesip.class.getCanonicalName());
        IDesip desip = IDesip.Stub.asInterface(binder);

        try {
            desip.addListener(this.getClass().getCanonicalName(), desipListener, new byte[] { Signals.AID_POWER_EVENT });
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }


    private IDesipListener desipListener = new IDesipListener.Stub() {
        @Override
        public boolean deliverMessage(ParcelableDesipMessage msg) throws RemoteException {
            Log.d("PowerEventDesipListener", "Desip listener deliverMessage()");

            startActivity(new Intent(MainApplication.this, AlertActivity.class));

            //TODO What does desip service do with return value?
            return false;
        }
    };
}
