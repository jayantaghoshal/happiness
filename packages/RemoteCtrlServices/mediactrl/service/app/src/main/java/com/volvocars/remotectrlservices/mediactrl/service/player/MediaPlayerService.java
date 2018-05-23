/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.mediactrl.service.player;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

import com.volvocars.remotectrlservices.mediactrl.service.R;
import com.volvocars.remotectrlservices.mediactrl.service.RemoteMediaStreamingService;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;


public class MediaPlayerService extends Service {

    public static final int STOP_STOPPED       = 0;
    public static final int PLAY_PLAYING       = 1;
    public static final int PAUSE_PAUSED       = 2;
    public static final int NEXT               = 5;
    public static final int PREVIOUS           = 6;

    private static final String TAG ="RemoteCtrl.MediaCtrl.Service.Player.MediaPlayerService";
    private static final String MEDIA_PLAYER_SERVICE_BIND_INTENT = ".MediaPlayerService.BIND";
    private static final String REMOTE_MEDIA_STREAMING_SERVICE_PACKAGE = "com.volvocars.remotectrlservices.mediactrl.service";
    private static final String REMOTE_MEDIA_STREAMING_SERVICE_BIND_INTENT = ".RemoteMediaStreamingService.BIND_MediaPlayerService";

    MediaPlayer mediaPlayer = null;


    private boolean mRemoteMediaStreamingServiceBound = false;
    private Messenger mRemoteMediaStreamingService = null;

    @Override
    public IBinder onBind(Intent intent) {
        if(intent.getAction().equals(MEDIA_PLAYER_SERVICE_BIND_INTENT)){
            bindRemoteMediaStreamingService();
            return mMessenger.getBinder();

        }
        return null;
    }


    @Override
    public void onCreate() {
        Log.v(TAG, "onCreate");
        super.onCreate();
        startForeground();

        //Create here the media player but do not start playing
        mediaPlayer = new MediaPlayer(this, mediaPlayerCallback);
    }

    private void startForeground() {
        Log.v(TAG,"Create notification and put service in foreground");
        NotificationChannel notificationChannel = new NotificationChannel("CHANNEL_ID","CHANNEL", NotificationManager.IMPORTANCE_DEFAULT);
        NotificationManager notificationManager = this.getSystemService(NotificationManager.class);
        notificationManager.createNotificationChannel(notificationChannel);

        Notification notification = new Notification.Builder(this,notificationChannel.getId())
                        .setContentTitle("Empty notification")
                        .setCategory(Notification.CATEGORY_SERVICE)
                        .setContentText("Foreground service media control")
                        .setSmallIcon(R.mipmap.ic_launcher)
                        .build();

        startForeground(/*dummy*/5,notification);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.v(TAG, "onDestroy");
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(TAG, "onUnbind");
        if(intent.getAction().equals(MEDIA_PLAYER_SERVICE_BIND_INTENT)){
            Log.v(TAG,"Remote MediaCtrl service unbound");
            mRemoteMediaStreamingServiceBound = false;
            return true;
        }
        return false;
    }

    private void bindRemoteMediaStreamingService(){
        if(mRemoteMediaStreamingServiceBound){
            Log.v(TAG,"Already bound to MediaPlayerService");
            return;
        }
        Log.v(TAG, "Binding to Remote Media Streaming Service");

        Intent binderIntent = new Intent(REMOTE_MEDIA_STREAMING_SERVICE_BIND_INTENT);
        binderIntent.setPackage(REMOTE_MEDIA_STREAMING_SERVICE_PACKAGE);
        if(!bindService(binderIntent, mRemoteMediaStreamingServiceConnection, Context.BIND_AUTO_CREATE)){
            Log.e(TAG, "Failed to bind to Media Player Service");
        }
    }

    private ServiceConnection mRemoteMediaStreamingServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mRemoteMediaStreamingServiceBound=true;
            mRemoteMediaStreamingService = new Messenger(service);
            Log.v(TAG,"RemoteMediaStreamingService connected");
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mRemoteMediaStreamingServiceBound=false;
            mRemoteMediaStreamingService = null;
            Log.v(TAG,"RemoteMediaStreamingService disconnected");
        }
    };

    private MediaPlayerCallback  mediaPlayerCallback = new MediaPlayerCallback(){
        @Override
        public void PlaybackStatusChanged(int playbackStatus, int fctAvailabilityStatus) {
            try{
                Log.v(TAG,"Notification MediaPlayer playbackStatus="+playbackStatus);
                mRemoteMediaStreamingService.send(Message.obtain(null,
                        RemoteMediaStreamingService.NOTIFICATION,
                        playbackStatus,
                        fctAvailabilityStatus));
            }catch(RemoteException e){
                Log.e(TAG,"RemoteException :",e);
            }
        }
    };


    //Message handler from RemoteMediaStreamingService

    final Messenger mMessenger = new Messenger(new IncomingHandler());

    private class IncomingHandler extends Handler{
        @Override
        public void handleMessage(Message msg) {
            boolean result=false;
            RemoteCtrlPropertyValue propValue = (RemoteCtrlPropertyValue)msg.obj;
            int playback = (int)propValue.getValue();
            RemoteCtrlPropertyValue returnPropValue = null;

            Log.v(TAG, "Request received #"+msg.what+" with value "+playback);

            switch(playback){
                case STOP_STOPPED: //STOP
                    Log.v(TAG, "STOP");
                    result = mediaPlayer.stop();
                    break;
                case PLAY_PLAYING: //PLAY
                    Log.v(TAG, "PLAY");
                    result = mediaPlayer.play();
                    break;
                case PAUSE_PAUSED: //PAUSE
                    Log.v(TAG, "PAUSE");
                    result = mediaPlayer.pause();
                    break;
                case NEXT: //NEXT
                    Log.v(TAG, "NEXT");
                    result = mediaPlayer.next();
                    break;
                case PREVIOUS: //PREVIOUS
                    Log.v(TAG, "PREVIOUS");
                    result = mediaPlayer.previous();
                    break;
                default: //OTHER VALUE NOT IN USE
                    Log.v(TAG, "Value not in use");
                    super.handleMessage(msg);
                    break;
            }

            if(result){
                returnPropValue = new RemoteCtrlPropertyValue(propValue.getPropertyId(), propValue.getAreaId(), RemoteCtrlHalPropertyStatus.AVAILABLE, propValue.getValue());
            }else{
                returnPropValue = new RemoteCtrlPropertyValue(propValue.getPropertyId(), propValue.getAreaId(), RemoteCtrlHalPropertyStatus.SYSTEM_ERROR, propValue.getValue());
            }

            //Send back response directly with same requestId and result
            try {
                Log.v(TAG,"sending response to gateway");
                mRemoteMediaStreamingService.send(Message.obtain(null,msg.what, returnPropValue));
            } catch(Exception e) {
                Log.e(TAG,"Exception :",e);
            }
        }
    }

}
