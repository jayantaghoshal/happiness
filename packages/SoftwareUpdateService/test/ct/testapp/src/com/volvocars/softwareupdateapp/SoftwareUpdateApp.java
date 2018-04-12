/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

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
import android.widget.Toast;
import com.volvocars.softwareupdate.*;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;
import com.volvocars.cloudservice.*;
import java.util.*;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.DefaultItemAnimator;
import android.support.v7.widget.Toolbar;
import android.graphics.Rect;

public class SoftwareUpdateApp extends AppCompatActivity implements ISoftwareUpdateApp {
    private static final String LOG_TAG = "SwUpdApp";

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
                Log.v(LOG_TAG, "UpdateState: Not registered, returning");
                return;
            }
            Log.v(LOG_TAG, "UpdateState " + state);
        }

        @Override
        public void UpdateSoftwareList(List<SoftwareInformation> software_list) {
            if (!registered) {
                Log.v(LOG_TAG, "UpdateSoftwareList: Not registered, returning");
                return;
            }
            Log.v(LOG_TAG, "SoftwareList:");
            swInfos.clear();
            for (SoftwareInformation si : software_list) {
                Log.v(LOG_TAG, "" + si.toString());
                swInfos.add(si);
            }

            updateAdapter();
        }

        @Override
        public void UpdateSoftware(SoftwareInformation software) {
            if (!registered) {
                Log.v(LOG_TAG, "UpdateSoftware: Not registered, returning");
                return;
            }
            for (SoftwareInformation si : swInfos) {
                Log.v(LOG_TAG, "UpdateSoftware: " + si.toString());
                if (si.softwareId.equals(software.softwareId)) {
                    swInfos.set(swInfos.indexOf(si), software);
                    break;
                }
            }
            updateAdapter();
        }

        @Override
        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, "ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");
        }

        public boolean registered = false;
    };

    private final ISoftwareUpdateManagerCallbackImpl callback = new ISoftwareUpdateManagerCallbackImpl();

    private final SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "onServiceConnected app");
            try {
                Log.v(LOG_TAG, "onServiceConnected app: Register - " + callback);
                softwareUpdateManager.RegisterSwUpdClient(callback);
                callback.registered = true;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error RegisterSwUpdClient");
            }
        }

        @Override
        public void onServiceDisconnected() {
            Log.v(LOG_TAG, "onServiceDisconnected app");
            try {
                Log.v(LOG_TAG, "onServiceDisconnected app: Unregister - " + callback);
                softwareUpdateManager.UnregisterSwUpdClient(callback);
                callback.registered = false;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error UnregisterSwUpdClient");
            }
        }
    };

    private void updateAdapter() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                adapter.notifyDataSetChanged();
            }
        });
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
            case R.id.getAvailableItem:
                try {
                    Log.v(LOG_TAG, "Sending GetSoftwareAssignments");
                    Intent intent = new Intent(context, AvailableAssignmentsActivity.class);
                    startActivity(intent);
                    return true;
                } catch (Exception e) {
                    Log.e(LOG_TAG, "GetSoftwareAssignments failed, remote exception");
                }
            default:
            }
            return false;
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate");
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
        Log.v(LOG_TAG, "onDestroy");
        try {
            Log.v(LOG_TAG, "onDestroy app: Unregister - " + callback);
            softwareUpdateManager.UnregisterSwUpdClient(callback);
            callback.registered = false;
        } catch (RemoteException e) {
            Log.e(LOG_TAG, "UnregisterSwUpdClient failed, remote exception");
        }
        softwareUpdateManager.disconnect();
    }

    //Create actionbar with settings icon
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.menu_actionbar_main, menu);

        return true;
    }

    //React on actionbar icon clicked (settings)
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.actionbarMain:
            Intent intent = new Intent(this, SettingsActivity.class);
            startActivity(intent);
            return true;
        case R.id.simSignals:
            Log.d(LOG_TAG, "SIM SINGALS");
            showPopupMenu((View) toolbar);
        default:
            // If we got here, the user's action was not recognized.
            // Invoke the superclass to handle it.
            return super.onOptionsItemSelected(item);

        }
    }

    private boolean sendSetSetting(Setting setting, boolean state) {
        if (setting.value != state) {
            Log.v(LOG_TAG, "State has changed to: " + state);
            try {
                softwareUpdateManager.SetSetting(setting);
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "sendSetSetting: RemoteException: " + e.getMessage());
            }
        } else {
            Log.v(LOG_TAG, "State has not changed... Nothing to do");
        }

        return setting.value;
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

    @Override
    protected void onStart() {
        super.onStart();
        Log.v(LOG_TAG, "OnStart");
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.v(LOG_TAG, "InBackground");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.v(LOG_TAG, "InForeground");
    }

    @Override
    public void commissionAssignment(String uuid) {
        try {
            softwareUpdateManager.CommissionAssignment(uuid);
        } catch (Exception e) {
            Log.e(LOG_TAG, e.getMessage());
        }
    }

    @Override
    public void getInstallNotification(String installationOrderId) {
        try {
            softwareUpdateManager.GetInstallNotification(installationOrderId);
        } catch (Exception e) {
            Log.e(LOG_TAG, e.getMessage());
        }
    }

    @Override
    public void showInstallationPopup(String installationOrderId) {
        try {
            softwareUpdateManager.ShowInstallationPopup(installationOrderId);
        } catch (Exception e) {
            Log.e(LOG_TAG, e.getMessage());
        }
    }
}
