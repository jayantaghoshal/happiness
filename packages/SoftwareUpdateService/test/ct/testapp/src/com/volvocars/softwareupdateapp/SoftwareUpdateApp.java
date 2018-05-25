/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import android.widget.PopupMenu;

import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.AssignmentType;
import com.volvocars.cloudservice.Query;
import com.volvocars.softwareupdate.*;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;
import java.util.*;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v4.app.NotificationCompat;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.DefaultItemAnimator;
import android.support.v7.widget.Toolbar;
import android.graphics.Rect;

public class SoftwareUpdateApp extends AppCompatActivity implements ISoftwareUpdateApp {
    private static final String LOG_TAG = "SoftwareUpdateApp";
    private static final String LOG_PREFIX = "[SoftwareUpdateApp]";

    private SoftwareUpdateManager softwareUpdateManager = null;

    private Context context = this;
    private RecyclerView recyclerView;
    public SoftwareInformationAdapter adapter;
    private ArrayList<SoftwareInformation> swInfos;
    private Toolbar toolbar;

    private static long lastClickedTime = 0;
    private static final int doubleClickInterval = 500;

    private class ISoftwareUpdateManagerCallbackImpl extends ISoftwareUpdateManagerCallback.Stub {
        @Override
        public void UpdateState(int state) {
            if (!registered) {
                Log.v(LOG_TAG, LOG_PREFIX + " UpdateState: Not registered, returning");
                return;
            }
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateState " + state);
        }

        @Override
        public void UpdateSoftwareList(List<SoftwareInformation> software_list) {
            if (!registered) {
                Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList: Not registered, returning");
                return;
            }
            Log.v(LOG_TAG, LOG_PREFIX + " SoftwareList:");
            boolean available = false;
            swInfos.clear();
            for (SoftwareInformation si : software_list) {
                Log.v(LOG_TAG, LOG_PREFIX + " " + si.toString() + "\n" + si.softwareState.name());
                swInfos.add(si);
                /*
                 * if (si.softwareAssignment.status = SoftwareAssignment.Status.COMMISSIONABLE)
                 * { createNotification(); }
                 */
            }

            updateAdapter();
        }

        @Override
        public void UpdateSoftware(SoftwareInformation software) {
            if (!registered) {
                Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftware: Not registered, returning");
                return;
            }
            for (SoftwareInformation si : swInfos) {
                Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftware: " + si.toString());
                if (si.softwareAssignment.id.equals(software.softwareAssignment.id)) {
                    swInfos.set(swInfos.indexOf(si), software);
                    break;
                }
            }
            updateAdapter();
        }

        @Override
        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, LOG_PREFIX + " ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");
        }

        /**
         * Notify client to show installation popup NOTE: this is a temporary solution,
         * remove once proper handling of system popups is implemented
         */
        @Override
        public void showInstallationPopup(SoftwareAssignment software) {
            if (!registered) {
                Log.v(LOG_TAG, LOG_PREFIX + " showInstallationPopUp: Not registered, returning");
                return;
            } else {
                runOnUiThread(() -> showInstallationDialog(software.name, software.installationOrder.id));
            }
        }

        public boolean registered = false;
    };

    private final ISoftwareUpdateManagerCallbackImpl callback = new ISoftwareUpdateManagerCallbackImpl();

    private final SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " onServiceConnected app");
            try {
                Log.v(LOG_TAG, LOG_PREFIX + " onServiceConnected app: Register - " + callback);
                softwareUpdateManager.RegisterSwUpdClient(callback);
                callback.registered = true;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " Error RegisterSwUpdClient");
            }
        }

        @Override
        public void onServiceDisconnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " onServiceDisconnected app");
            try {
                Log.v(LOG_TAG, LOG_PREFIX + " onServiceDisconnected app: Unregister - " + callback);
                softwareUpdateManager.UnregisterSwUpdClient(callback);
                callback.registered = false;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " Error UnregisterSwUpdClient");
            }
        }
    };

    private void updateAdapter() {
        runOnUiThread(() -> adapter.notifyDataSetChanged());
    }

    /**
     * Showing popup menu when tapping on cloud icon in toolbar
     */
    private void showPopupMenu(View view) {
        // inflate menu
        PopupMenu popup = new PopupMenu(context, view, Gravity.END);
        MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.menu_signals, popup.getMenu());
        popup.setOnMenuItemClickListener(new MyMenuItemClickListener());
        popup.show();
    }

    /**
     * Click listener for popup menu items
     */
    class MyMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {

        @Override
        public boolean onMenuItemClick(MenuItem menuItem) {
            switch (menuItem.getItemId()) {
            case R.id.getUpdatesItem:
                try {
                    Log.v(LOG_TAG, LOG_PREFIX + " Sending GetSoftwareUpdates");
                    Query query = new Query();
                    softwareUpdateManager.GetSoftwareAssignment(query, AssignmentType.UPDATE);
                    Snackbar.make(findViewById(R.id.rootLayout), "Calling GetSoftwareUpdates", Snackbar.LENGTH_SHORT)
                            .show();
                    return true;
                } catch (Exception e) {
                    Log.e(LOG_TAG, LOG_PREFIX + " GetSoftwareAssignments failed, remote exception");
                }
            case R.id.getAccessoriesItem:
                try {
                    Log.v(LOG_TAG, LOG_PREFIX + " Sending GetSoftwareAccessories");
                    Query query = new Query();
                    softwareUpdateManager.GetSoftwareAssignment(query, AssignmentType.ACCESSORY);
                    Snackbar.make(findViewById(R.id.rootLayout), "Calling GetSoftwareAccessories",
                            Snackbar.LENGTH_SHORT).show();
                    return true;
                } catch (Exception e) {
                    Log.e(LOG_TAG, LOG_PREFIX + " GetSoftwareAssignments failed, remote exception");
                }
            default:
            }
            return false;
        }
    }

    private void showInstallationDialog(String name, String id) {
        Log.v(LOG_TAG, LOG_PREFIX + " showInstallationDialog");

        AlertDialog alertDialog = new AlertDialog.Builder(this).create();

        // Setting Dialog Title
        alertDialog.setTitle("SoftwareUpdate");

        // Setting Dialog Message
        alertDialog.setMessage(name + " is ready to be installed. Install now?");

        // Setting Icon to Dialog
        alertDialog.setIcon(R.drawable.ic_get_app_36dp);

        // Setting OK Button
        alertDialog.setButton(AlertDialog.BUTTON_POSITIVE, "Install", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                try {
                    Snackbar.make(findViewById(R.id.rootLayout),
                            "Calling OnInstallationPopup(INSTALL, " + id + ")",
                            Snackbar.LENGTH_SHORT).show();
                    softwareUpdateManager.OnInstallationPopup(InstallOption.INSTALL,
                            id);
                } catch (Exception e) {
                    Log.e(LOG_TAG, LOG_PREFIX + e.getMessage());
                }
            }
        });

        alertDialog.setButton(AlertDialog.BUTTON_NEGATIVE, "CANCEL", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                try {
                    Snackbar.make(findViewById(R.id.rootLayout),
                            "Calling OnInstallationPopup(CANCEL, " + id + ")",
                            Snackbar.LENGTH_SHORT).show();
                    softwareUpdateManager.OnInstallationPopup(InstallOption.CANCEL,
                            id);
                } catch (Exception e) {
                    Log.e(LOG_TAG, LOG_PREFIX + e.getMessage());
                }
            }
        });

        alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "Postpone ", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                try {
                    Snackbar.make(findViewById(R.id.rootLayout),
                            "Calling OnInstallationPopup(POSTPONE, " + id + ")",
                            Snackbar.LENGTH_SHORT).show();
                    softwareUpdateManager.OnInstallationPopup(InstallOption.POSTPONE,
                            id);
                } catch (Exception e) {
                    Log.e(LOG_TAG, LOG_PREFIX + e.getMessage());
                }
            }
        });

        // Showing Alert Message
        alertDialog.show();
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, LOG_PREFIX + " onCreate");
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);

        setContentView(R.layout.activity_main);

        Toolbar myToolbar = (Toolbar) findViewById(R.id.toolbar);
        myToolbar.setTitle("Software Update");

        setSupportActionBar(myToolbar);

        recyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        toolbar = (Toolbar) findViewById(R.id.toolbar);

        swInfos = new ArrayList<SoftwareInformation>();
        adapter = new SoftwareInformationAdapter(this, swInfos, this);

        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, true);
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.addItemDecoration(new VerticalSpaceItemDecoration(10));
        recyclerView.setAdapter(adapter);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, LOG_PREFIX + " onDestroy");
        try {
            Log.v(LOG_TAG, LOG_PREFIX + " onDestroy app: Unregister - " + callback);
            softwareUpdateManager.UnregisterSwUpdClient(callback);
            callback.registered = false;
        } catch (RemoteException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " UnregisterSwUpdClient failed, remote exception");
        }
        softwareUpdateManager.disconnect();
    }

    // Create actionbar with settings icon
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.menu_actionbar_main, menu);

        return true;
    }

    // React on actionbar icon clicked (settings)
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.settings:
            Intent intent = new Intent(this, SettingsActivity.class);
            startActivity(intent);
            return true;
        case R.id.simSignals:
            showPopupMenu((View) toolbar);
        default:
            // If we got here, the user's action was not recognized.
            // Invoke the superclass to handle it.
            return super.onOptionsItemSelected(item);

        }
    }

    public class VerticalSpaceItemDecoration extends RecyclerView.ItemDecoration {

        private final int verticalSpaceHeight;

        public VerticalSpaceItemDecoration(int verticalSpaceHeight) {
            this.verticalSpaceHeight = verticalSpaceHeight;
        }

        @Override
        public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
            outRect.top = verticalSpaceHeight;
        }

    }

    private void createNotification() {
        NotificationManager mgr = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        // Create a notification channel (does the id need to be unique for the
        // package?)
        NotificationChannel notificationChannel = new NotificationChannel("12345", "softwareupdate",
                NotificationManager.IMPORTANCE_HIGH);
        mgr.createNotificationChannel(notificationChannel);

        // Build the notification
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, "12345");

        builder.setSmallIcon(R.drawable.ic_cloud_white_18dp); // mandatory setting
        builder.setContentTitle("Software updates"); // mandatory setting
        builder.setContentText("A new software update can be downloaded"); // mandatory setting

        // Cancel notification on tap
        builder.setAutoCancel(true);

        int notificationId = 1111;
        // Start main activity when the notification is tapped
        Intent intent = new Intent(this, SoftwareUpdateApp.class);
        // Cancel any pending intent that have already been fired when the user taps the
        // notification
        PendingIntent pendingIntent = PendingIntent.getActivity(this, notificationId, intent,
                PendingIntent.FLAG_CANCEL_CURRENT);

        builder.setContentIntent(pendingIntent);
        // Notify NotficationManager about the notification
        Notification notification = builder.build();

        mgr.notify(notificationId, notification);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.v(LOG_TAG, LOG_PREFIX + " OnStart");
        // showInstallationDialog();
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.v(LOG_TAG, LOG_PREFIX + " InBackground");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.v(LOG_TAG, LOG_PREFIX + " InForeground");
    }

    @Override
    public void commissionAssignment(String uuid) {
        try {
            Snackbar.make(findViewById(R.id.rootLayout), "Calling CommissionAssignment(" + uuid + ")",
                    Snackbar.LENGTH_SHORT).show();
            softwareUpdateManager.CommissionAssignment(uuid);
        } catch (Exception e) {
            Log.e(LOG_TAG, LOG_PREFIX + e.getMessage());
        }
    }

    @Override
    public void getInstallNotification(String installationOrderId) {
        try {
            Snackbar.make(findViewById(R.id.rootLayout), "Calling GetInstallNotification(" + installationOrderId + ")",
                    Snackbar.LENGTH_SHORT).show();
            softwareUpdateManager.GetInstallNotification(installationOrderId);
        } catch (Exception e) {
            Log.e(LOG_TAG, LOG_PREFIX + e.getMessage());
        }
    }
}
