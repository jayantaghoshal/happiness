/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.audioctrl;

import android.content.Context;
import android.content.ComponentName;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.ServiceConnection;
import java.util.Timer;
import java.util.TimerTask;

public class AudioCtrlService extends Service {

  private static final String TAG = "RemoteCtrl.AudioCtrl.Service";
  private static final String APP_SERVICE_PACKAGE_NAME = "com.volvocars.remotectrlappservices.audioctrl";
  private static final String EXPECTED_INTENT = "com.volvocars.remotectrlservices.audioctrl.AudioCtrlService.ACTION_VOLUME_CTRL";
  private static final String OUTGOING_INTENT = "com.volvocars.remotectrlappservices.audioctrl.AudioCtrlAppService.ACTION_VOLUME_CTRL";
  private ApiHandler mServiceInterfaceImpl = null;
  private HalHandler mHalHandler = null;
  private IAudioCtrlAppService mAppService = null;
  private DeathRecipient mBinderDeath = null;
  /* @TODO Reduce wait time when AppService is delivered.
  Wait time is increased to avoid being too chatty. */
  private long mConnRetryWaitTime = 10000;
  private boolean mBindingCompleted = false;
  private Timer mStarterTimer;
  private TimerTask mStarterTimerTask;

  /**
   *  Abstract methods - Service class
   */
  @Override
  public void onCreate() {
    Log.v(TAG, "onCreate");
    super.onCreate();

    // Create aidl binder interface for service's client.
    if(mServiceInterfaceImpl == null) {
      mServiceInterfaceImpl = new ApiHandler(this);
    }

    // Create HAL handler instance
    if(mHalHandler == null)
    {
      mHalHandler = new HalHandler();
      mHalHandler.init();
      mServiceInterfaceImpl.init(mHalHandler);
    }

    mBinderDeath = new DeathRecipient(mHalHandler);
  }

  @Override
  public int onStartCommand(Intent intent, int flags, int startId) {
    Log.v(TAG, "onStartCommand");
    retryStartAppService();
    return START_STICKY;
  }

  @Override
  public void onDestroy() {
    Log.v(TAG, "onDestroy");
    super.onDestroy();
    unregisterFromNativeService();
    unbindService(mAppServiceConnection);
  }

  private void unregisterFromNativeService(){
    mHalHandler.mClearToRegisterToDaemon = false;
    mHalHandler.unregisterFromNativeService();
  }

  private ServiceConnection mAppServiceConnection = new ServiceConnection() {

    public void onServiceConnected(ComponentName name, IBinder service) {
      Log.v(TAG, "AppService connected");
      try{
        service.linkToDeath(mBinderDeath, 0);
      }catch(RemoteException ex){
        Log.e(TAG, "link to death error: " + ex.getMessage());
      }
      mAppService = IAudioCtrlAppService.Stub.asInterface(service);
      mHalHandler.setAppService(mAppService);
      mHalHandler.mClearToRegisterToDaemon = true;
      mHalHandler.registerToNativeService();
      mBindingCompleted = true;
      mStarterTimer.cancel();
      mStarterTimerTask.cancel();
    }

    public void onServiceDisconnected(ComponentName name) {
      Log.e(TAG, "AppService has unexpectedly disconnected");
      mAppService = null;
      unregisterFromNativeService();
      mBindingCompleted = false;
      retryStartAppService();
    }
  };

   // When a client(AppService) connects to this service, we return the binder interface
  @Override
  public IBinder onBind(Intent intent) {
    Log.v(TAG, "onBind");
    /* NOTE(Samuel.Idowu) : A single service can be used to offer various binder interface
                          .e.g return VolumeCtrlAPI or ClimateCtrlAPI based on intent action. */
    if(intent.getAction().equals(EXPECTED_INTENT)){
      Log.v(TAG, "Bind on RemoteCtrlServices.AudioCtrlService");
      bindToAppService();
      return mServiceInterfaceImpl;
    }else{
      Log.d(TAG, "Trying to bind with unknown action: " + intent.getAction());
      return null;
    }
  }

  private void bindToAppService(){
    Intent appServiceIntent = new Intent(OUTGOING_INTENT);
    appServiceIntent.setPackage(APP_SERVICE_PACKAGE_NAME);
    bindService(appServiceIntent, mAppServiceConnection, Context.BIND_AUTO_CREATE);
  }

  private void startAppService()
  {
    Intent appServiceIntent = new Intent(OUTGOING_INTENT);
    appServiceIntent.setPackage(APP_SERVICE_PACKAGE_NAME);
    this.startService(appServiceIntent);
  }

  private void retryStartAppService(){
    Log.v(TAG, "StartAppService");
    if(mBindingCompleted){
      Log.v(TAG,"Binding has been completed");
      return;
    }else{
      startAppService();

      mStarterTimer = new Timer();
      mStarterTimerTask = new TimerTask() {
        @Override
        public void run() {
          Log.v(TAG, "Retrying to startAppService ...");
          retryStartAppService();
        }
      };
      mStarterTimer.schedule(mStarterTimerTask, mConnRetryWaitTime);
    }
  }

  final class DeathRecipient implements IBinder.DeathRecipient {

    HalHandler halHandler;

    DeathRecipient(HalHandler handler) {
      this.halHandler = handler;
    }

    @Override
    public void binderDied() {
      Log.e(TAG, "AppService Binder died");
      unregisterFromNativeService();
    }
  }
}
