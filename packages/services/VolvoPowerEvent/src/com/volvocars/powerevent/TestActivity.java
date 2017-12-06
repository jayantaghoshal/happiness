package com.volvocars.powerevent;

import android.app.Activity;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.view.View;

import com.delphi.desip.IDesip;
import com.delphi.desip.IDesipListener;

//TODO: TestActivity is a non-production helper screen which should be removed later. Also remove activity_test.xml and the activity declaration from AndroidManifest.xml
public class TestActivity extends Activity {
    private static final String listenerUid = "powereventtestlistener";

    private IDesip desip;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);

        findViewById(R.id.button1).setOnClickListener(button1ClickListener);
        findViewById(R.id.button2).setOnClickListener(button2ClickListener);
        findViewById(R.id.button3).setOnClickListener(button3ClickListener);

        IBinder binder = ServiceManager.getService(IDesip.class.getCanonicalName());
        desip = IDesip.Stub.asInterface(binder);
    }

    private View.OnClickListener button1ClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            try {
                desip.addListener(listenerUid, desipListener, new byte[] { Signals.AID_POWER_EVENT });
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    };

    private View.OnClickListener button2ClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            try {
                desip.removeListener(listenerUid);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    };

    private View.OnClickListener button3ClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
//            try {
//                desip.send(new ParcelableDesipMessage());
//            } catch (RemoteException e) {
//                e.printStackTrace();
//            }
        }
    };

    private IDesipListener desipListener = new IDesipListener.Stub() {
        @Override
        public boolean deliverMessage(ParcelableDesipMessage msg) throws RemoteException {
            Log.d("desipactivity", "Desip listener deliverMessage()");
            return false;
        }
    };
}
