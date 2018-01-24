/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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
    private static final String LOGTAG = "VolvoPowerEvent";

    @Override
    public void onCreate() {
        super.onCreate();

        /*
         * Disabled because it is broken.
         *
        IBinder binder = ServiceManager.getService(IDesip.class.getCanonicalName());
        Desip desip = IDesip.Stub.asInterface(binder);

        try {
            desip.addListener(this.getClass().getCanonicalName(), desipListener, new byte[] { Signals.AID_POWER_EVENT });
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        */
    }

/*
    private IDesipListener desipListener = new IDesipListener.Stub() {
        @Override
        public boolean deliverMessage(ParcelableDesipMessage msg) throws RemoteException {
            Log.d(LOGTAG, String.format("Received ParcelableDesipMessage - %s", msg));

            startActivity(new Intent(MainApplication.this, AlertActivity.class));

            //TODO What does desip service do with return value?
            return false;
        }
    };
*/
}
