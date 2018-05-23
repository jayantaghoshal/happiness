/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.KeyguardManager;
import android.app.Service;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.UserInfo;
import android.hidl.manager.V1_0.IServiceManager;
import android.hidl.manager.V1_0.IServiceNotification;
import android.os.Binder;
import android.os.IBinder;
import android.os.IHwBinder;
import android.os.RemoteException;
import android.os.UserManager;
import android.util.Log;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.util.NoSuchElementException;
import java.util.List;
import java.util.Optional;

import vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager;

/**
 * UserSwitchService starts the userswitch service that connects HMI client to Hal server
 * TODO: After TrustService is fixed, need to start the service as a TrustService
 */
public class UserSwitchService extends Service {
    public static final String TAG = UserSwitchService.class.getSimpleName();
    private static final int TIME_WAIT_MS = 500;
    private final Object mLock = new Object();
    private ICarProfileManager carProfileManager;
    private UserSwitchManagerHMI userSwitchManagerHMI;
    private ActivityManager activityManager;
    private UserManager userManager;
    private String mProfileManagerInterfaceName;
    private ProfileManagerDeathRecipient mProfileDeathRecipient =
            new ProfileManagerDeathRecipient();

    private UserInfo defaultGuest;

    @Override
    public void onCreate() {
        Log.i(TAG, "onCreate");
        activityManager = getBaseContext().getSystemService(ActivityManager.class);
        userManager = getBaseContext().getSystemService(UserManager.class);
        userSwitchManagerHMI = new UserSwitchManagerHMI();

        List<UserInfo> listUser = userManager.getUsers(false);
        String guestUserName = getString(R.string.default_guest_name);
        Optional<UserInfo> result =
                listUser.stream().filter(u -> u.name.equals(guestUserName)).findAny();
        if (!result.isPresent()) {
            // TODO (ARTINFO-507 - Guest Account) Change user flag for the defalut guest.
            VolvoUser user = new VolvoUser(-1, guestUserName, VolvoUser.FLAG_ADMIN);
            defaultGuest = userManager.createUser(user.name, user.flags);
            Log.d(TAG, "Created default guest: " + defaultGuest);
        } else {
            defaultGuest = result.get();
        }

        Log.v(TAG, "Registering for the serviceNotification service.");
        try {
            ServiceNotification serviceNotification = new ServiceNotification();

            boolean ret = registerForNotification(serviceNotification);
            while (!ret) {
                Thread.sleep(TIME_WAIT_MS);
                ret = registerForNotification(serviceNotification);
                Log.w(TAG, "Failed to register service start notification, retrying ...");
            }
        } catch (RemoteException e) {
            Log.d(TAG, "Failed to register service start notification", e);
            return;
        } catch (InterruptedException ex) {
            Log.w(TAG, "Failed to register thread sleep Interrupted.");
            Thread.currentThread().interrupted();
            return;
        }

        connectToProfileManager();
        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.i(TAG, "onDestroy");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return userSwitchManagerHMI;
    }

    /**
     * Switch user
     */
    public void switchUser(int androidUserId) {
        Log.d(TAG, "switchUser is called with androidUserId: " + androidUserId);
        if (androidUserId == VolvoUser.NOT_DEFINED) {
            Log.d(TAG, "unknown user received, switching to guest user");
            androidUserId = defaultGuest.id;
        }

        final boolean switchStatus = activityManager.switchUser(androidUserId);
        Log.d(TAG, "Current user: " + userManager.getUserName());
        Log.d(TAG, "Switch user status: " + switchStatus);

        // CEM changed but android user not, therefore fallback to guest
        if (!switchStatus) {
            // TODO (Torbjörn Sandsgård) Add falut handling if this happens.
            // Possibly we could retry a couple of times. We could also update the
            // API towards HAL so that we could make sure that the mapping between androidUser and
            // profileId is in sync.
            try {
                userSwitchManagerHMI.switchUser(
                        String.valueOf(defaultGuest.id), new IUserSwitchCallBack.Stub() {
                            @Override
                            public void handleResponse(UserSwitchResult result)
                                    throws RemoteException {
                                Log.d(TAG, "Switch user status from CEM: " + switchStatus);
                            }
                        });
            } catch (RemoteException e) { // TODO: smart way handling async users
                Log.e(TAG, " Something really went wrong" + switchStatus, e);
            }
        }

        // TODO(PSS370-14385): ...  that this has to be removed
        //        KeyguardManager keyguardManager = (KeyguardManager)getSystemService(Activity
        // .KEYGUARD_SERVICE);
        //        KeyguardManager.KeyguardLock lock = keyguardManager.newKeyguardLock(Activity
        // .KEYGUARD_SERVICE);
        //        lock.disableKeyguard();
        // keyguardManager.requestDismissKeyguard(new DismissListener());    //requires activity
    }

    /**
     * Create User and pair with the Profile
     */
    public void createUser(VolvoUser volvoUser) throws RemoteException {
        UserInfo userInfo = userManager.createUser(volvoUser.name, volvoUser.flags);
        Log.d(TAG, "Created UserInfo userInfo: " + userInfo);
        if (userInfo != null) {
            boolean result =
                    carProfileManager.pairAndroidUserToUnusedVehicleProfile(userInfo.id + "");
            Log.d(TAG, "pairAndroidUserToUnusedVehicleProfile has a result: " + result);

            if (!result) {
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setMessage("No available profiles").setTitle("Warning!");
                AlertDialog dialog = builder.create();
                userSwitchManagerHMI.getCallBackMap(volvoUser.id)
                        .handleResponse(UserSwitchResult.ERROR);
            } else {
                userSwitchManagerHMI.getCallBackMap(volvoUser.id)
                        .handleResponse(UserSwitchResult.SUCCESS);
            }
        } else {
            Log.e(TAG, "No User info");
            userSwitchManagerHMI.getCallBackMap(volvoUser.id)
                    .handleResponse(UserSwitchResult.ERROR);
        }
    }

    private boolean registerForNotification(ServiceNotification serviceNotification)
            throws RemoteException {
        return IServiceManager.getService().registerForNotifications(
                ICarProfileManager.kInterfaceName, "", serviceNotification);
    }

    private void connectToProfileManager() {
        synchronized (mLock) {
            if (carProfileManager != null) {
                return;
            }
            ICarProfileManager profileManager = null;
            try {
                profileManager = ICarProfileManager.getService();
                if (profileManager != null) {
                    profileManager.linkToDeath(mProfileDeathRecipient, 0);
                    carProfileManager = profileManager;
                    userSwitchManagerHMI.init(
                            UserSwitchService.this, carProfileManager, userManager);
                }
            } catch (RemoteException e) {
                Log.e(TAG, "CarProfileManager service not responding", e);
            } catch (NoSuchElementException e) {
                Log.e(TAG, "CarProfileManager service not registered yet");
            }

            try {
                mProfileManagerInterfaceName = profileManager.interfaceDescriptor();
            } catch (RemoteException | NullPointerException e) { // handles also null pointer
                throw new IllegalStateException(
                        "Unable to get Profile Manager HAL interface descriptor", e);
            }
        }
    }

    @Override
    protected void dump(FileDescriptor fd, PrintWriter writer, String[] args) {
        if (checkCallingOrSelfPermission(android.Manifest.permission.DUMP)
                != PackageManager.PERMISSION_GRANTED) {
            writer.println("Permission Denial: can't dump from from pid=" + Binder.getCallingPid()
                    + ", uid=" + Binder.getCallingUid() + " without permission "
                    + android.Manifest.permission.DUMP);
            return;
        }
        if (args == null || args.length == 0) {
            writer.println("*dump profile manager service*");
            writer.println("Profile Manager HAL Interface: " + mProfileManagerInterfaceName);
            writer.println("**Debug info**");
            writer.println("Profile Manager HAL reconnected: " + mProfileDeathRecipient.deathCount
                    + " times.");
        }
    }

    /**
     * Will be used after TrustService is ready
     */
    class DismissListener extends KeyguardManager.KeyguardDismissCallback {
        public void onDismissCancelled() {
            Log.i(TAG, "onDismissCancelled");
        }

        public void onDismissError() {
            Log.i(TAG, "onDismissError");
        }

        public void onDismissSucceeded() {
            Log.i(TAG, "onDismissSucceeded");
        }
    }

    /**
     * Validation and crash handling
     */

    private class ProfileManagerDeathRecipient implements IHwBinder.DeathRecipient {
        private int deathCount = 0;

        @Override
        public void serviceDied(long cookie) {
            Log.w(TAG, "Profile Manager HAL died.");

            // Remove a previously registered death notification. The recipient will no longer be
            // called if this object dies.
            try {
                carProfileManager.unlinkToDeath(this);
            } catch (RemoteException e) {
                Log.e(TAG, "Failed to unlinkToDeath", e); // TAG and continue.
            }
            // Clear all from the used places
            carProfileManager = null;
            userSwitchManagerHMI.clear();

            Log.i(TAG, "Notifying User Profile Manager to reconnect...");
        }
    }

    final class ServiceNotification extends IServiceNotification.Stub {
        @Override
        public void onRegistration(String fqName, String name, boolean preexisting) {
            synchronized (mLock) {
                Log.d(TAG, "IServiceNotification service started " + fqName + " " + name);
                connectToProfileManager();
            }
        }
    }
}
