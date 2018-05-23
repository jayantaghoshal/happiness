/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.mediactrl.service;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyResponseService;
import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;

public class RemoteMediaStreamingService extends Service {

    public static final int NOTIFICATION = -1;

    private static final String TAG = "RemoteCtrl.MediaCtrl.Service.RemoteMediaStreamingService";
    private static final String INTERFACE = "eth1";

    private static final String REMOTE_MEDIA_SERVICE_INTERFACE_PACKAGE = "com.volvocars.remotectrlservices.mediactrl.gateway";
    private static final String REMOTE_MEDIA_SERVICE_RESPONSE_BIND_INTENT = ".RemoteMediaGateway.BIND";
    private static final String REMOTE_MEDIA_SERVICE_BIND_INTENT = ".RemoteMediaStreamingService.BIND";
    private static final String MEDIA_PLAYER_SERVICE_BIND_INTENT = ".MediaPlayerService.BIND";
    private static final String REMOTE_MEDIA_STREAMING_SERVICE_BIND_INTENT = ".RemoteMediaStreamingService.BIND_MediaPlayerService";


    private boolean mRemoteMediaResponseServiceBound = false;
    private IRemoteCtrlPropertyResponseService mRemoteMediaControlResponseService = null;

    private Messenger mMediaPlayerMessenger = null;
    private boolean mMediaPlayerServiceBound = false;

    @Override
    public void onCreate() {
        Log.v(TAG, "MediaStreamingService onCreate");

        getProcessNetwork();
    }

    private void getProcessNetwork() {
        Log.v(TAG,"getProcessNetwork");

        ConnectivityManager cm = (ConnectivityManager) this.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkRequest networkRequest = new NetworkRequest.Builder().addTransportType(NetworkCapabilities.TRANSPORT_ETHERNET).build();
        cm.requestNetwork(networkRequest,
        new ConnectivityManager.NetworkCallback(){
            @Override
            public void onAvailable(Network network) {
                boolean eth1Connected = false;
                Log.v(TAG, "onAvailable");
                Network[] networks = cm.getAllNetworks();
                for (Network nw : networks) {
                    LinkProperties lp = cm.getLinkProperties(nw);
                    Log.v(TAG, "Available network: " + lp.getInterfaceName());
                    if (lp.getInterfaceName().equals(INTERFACE)) {
                        eth1Connected = cm.bindProcessToNetwork(nw);
                        Log.d(TAG, "BindingProcessToNetwork " + lp.getInterfaceName() + " " + eth1Connected);
                    }
                }
                if (!eth1Connected) {
                    Log.e(TAG, INTERFACE+" interface not found");
                }else{
                    bindMediaPlayerService();
                }
            }
        });
    }


    @Override
    public IBinder onBind(Intent intent) {
        if(intent.getAction().equals(REMOTE_MEDIA_SERVICE_BIND_INTENT)){
            mRemoteMediaResponseServiceBound=true;
            return mRemoteMediaControlService;
        }
        if(intent.getAction().equals(REMOTE_MEDIA_STREAMING_SERVICE_BIND_INTENT)){
            return mMessenger.getBinder();
        }
        return null;
    }

    //Start command coming from another app
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "onStart command");
        return Service.START_STICKY;
    }

    private void bindMediaPlayerService(){
        if(mMediaPlayerServiceBound){
            Log.v(TAG,"Already bound to MediaPlayerService");
            return;
        }
        Log.v(TAG, "Binding to Media Player Service");

        Intent binderIntent = new Intent(MEDIA_PLAYER_SERVICE_BIND_INTENT);
        binderIntent.setPackage(this.getPackageName());
        if(!bindService(binderIntent, mMediaPlayerServiceConnection, Context.BIND_AUTO_CREATE)){
            Log.e(TAG, "Failed to bind to Media Player Service");
        }

    }

    private void bindRemoteMediaCtrlResponseService(){
        if (mRemoteMediaResponseServiceBound) {
            Log.v(TAG, "Already bound to IRemoteMediaControlResponseService");
            return;
        }
            Log.v(TAG, "Binding Remote Media Ctrl Response Service");

            Intent bindServiceIntent = new Intent(REMOTE_MEDIA_SERVICE_RESPONSE_BIND_INTENT);
            bindServiceIntent.setPackage(REMOTE_MEDIA_SERVICE_INTERFACE_PACKAGE);
            if (!bindService(bindServiceIntent, mRemoteMediaCtrlResponseServiceConnection, Context.BIND_AUTO_CREATE)) {
                Log.e(TAG, "Failed to bind RemoteClimateService");

        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.v(TAG,"onDestroy");
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(TAG,"onUnbind");
        if(intent.getAction().equals(REMOTE_MEDIA_SERVICE_BIND_INTENT)){
            Log.v(TAG,"Remote MediaCtrl service unbound");
            mRemoteMediaResponseServiceBound = false;
            return true;
        }
        return false;
    }


    private ServiceConnection mRemoteMediaCtrlResponseServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.v(TAG, "IRemoteMediaResponseControlService connected.");
            mRemoteMediaControlResponseService = IRemoteCtrlPropertyResponseService.Stub.asInterface(service);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.v(TAG, "IRemoteMediaResponseControlService disconnected.");
            mRemoteMediaControlResponseService = null;
        }
    };

    private ServiceConnection mMediaPlayerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.v(TAG, "MediaPlayerService connected.");
            mMediaPlayerServiceBound=true;
            mMediaPlayerMessenger= new Messenger(service);
            bindRemoteMediaCtrlResponseService();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.v(TAG, "MediaPlayerService disconnected.");
            mMediaPlayerMessenger=null;
            mMediaPlayerServiceBound=false;
            unbindRemoteMediaCtrlResponseService();
        }
    };

    private void unbindRemoteMediaCtrlResponseService(){
        unbindService(mRemoteMediaCtrlResponseServiceConnection);
        Log.v(TAG,"RemoteMediaCtrlResponse service unbound");
    }



    // IMPLEMENTATION OF IRemoteMediaControlService Interface //

    private final IRemoteCtrlPropertyService.Stub
            mRemoteMediaControlService = new IRemoteCtrlPropertyService.Stub() {

        @Override
        public void setProperty(int requestIdentifier, RemoteCtrlPropertyValue remoteCtrlPropertyValue) throws RemoteException {
            try {
                int playbackStatus = (int) remoteCtrlPropertyValue.getValue();
                Log.v(TAG, "Received request "+requestIdentifier+" with status "+playbackStatus);
                mMediaPlayerMessenger.send(Message.obtain(null, requestIdentifier, remoteCtrlPropertyValue));
            } catch (RemoteException e) {
                Log.e(TAG,"RemoteException",e);
            }
        }

        @Override
        public void getProperty(int requestIdentifier, RemoteCtrlPropertyValue remoteCtrlPropertyValue) throws RemoteException {

        }
    };

    // Messenger to receive for MediaPlayerService -> RemoteMediaStreamingService communication

    final Messenger mMessenger = new Messenger(new IncomingHandler());

    private class IncomingHandler extends Handler{
        @Override
        public void handleMessage(Message msg) {
            try {
                Log.v(TAG, "New message from MediaPlayer");
                if(msg.what==NOTIFICATION){ //NOTIFICATION
                    RemoteCtrlPropertyValue remoteCtrlPropertyValue = new RemoteCtrlPropertyValue<Integer>(
                            RemoteCtrlHalProperty.REMOTECTRLHAL_MEDIA_STREAM_CONTROL,
                            0,
                            (Integer)msg.arg2,
                            (Integer)msg.arg1);
                    Log.v(TAG,"NOTIFICATION"+remoteCtrlPropertyValue);

                    mRemoteMediaControlResponseService.notifyPropertyChanged(remoteCtrlPropertyValue);
                }else{  //RESPONSE
                    Log.v(TAG,"RESPONSE"+msg);
                    int requestId = msg.what;
                    mRemoteMediaControlResponseService.sendSetPropertyResponse(requestId, (RemoteCtrlPropertyValue)msg.obj);
                }
            }catch(RemoteException e) {
                Log.v(TAG, "RemoteExcetion : ",e);
            }

        }
    }


}
