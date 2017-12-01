package com.volvocars.keypaneleventsniffer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

import static android.view.KeyEvent.keyCodeToString;

public class MainActivity extends Activity {
    public static final String LOG_TAG = "Test_KeyPanelEventSniffer";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(LOG_TAG,"Init");
    }
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(LOG_TAG,"InBackground");
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(LOG_TAG,"InForeground");
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.d(LOG_TAG,"{\"event\":\"keydown\", \"keycode\":\""+ keyCodeToString(keyCode) + "\"}");
        return super.onKeyDown(keyCode, event);
    }
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        Log.d(LOG_TAG,"{\"event\":\"keyup\", \"keycode\":\""+ keyCodeToString(keyCode) + "\"}");
        return super.onKeyDown(keyCode, event);
    }
}


