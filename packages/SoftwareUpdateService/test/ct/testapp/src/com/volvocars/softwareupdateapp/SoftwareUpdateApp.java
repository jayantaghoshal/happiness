/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.content.Context;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import com.volvocars.softwareupdate.*;
import com.volvocars.cloudservice.*;
import java.util.*;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.DefaultItemAnimator;

import android.graphics.Rect;

public class SoftwareUpdateApp extends AppCompatActivity implements ISoftwareUpdateApp {
    private static final String LOG_TAG = "SwUpdApp";
    private SoftwareUpdateManager softwareUpdateManager = null;

    private Context context = this;
    private RecyclerView recyclerView;
    public SoftwareInformationAdapter adapter;
    private ArrayList<SoftwareInformation> swInfos;

    private FloatingActionButton actionsFab;
    private FloatingActionButton availableFab;
    private FloatingActionButton commissionedFab;
    private LinearLayout layoutFabAvailable;
    private LinearLayout layoutFabCommissioned;

    private boolean fabExpanded = false;

    private ISoftwareUpdateManagerCallback callback = new ISoftwareUpdateManagerCallback.Stub() {
        @Override
        public void UpdateState(int state) {
            Log.v(LOG_TAG, "UpdateState " + state);
            if (state == 1) {
                actionsFab.setEnabled(true);
            } else {
                actionsFab.setEnabled(false);
            }
        }

        @Override
        public void UpdateSoftwareList(List<SoftwareInformation> software_list) {
            swInfos.clear();
            for(SoftwareInformation si : software_list) {
                swInfos.add(si);
            }

            updateAdapter();
        }

        @Override
        public void UpdateSoftwareState(String uuid, int state) {
            Log.v(LOG_TAG, "UpdateSoftwareState");
        }

        @Override
        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, "ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");
        }
    };

    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "onServiceConnected app");
            try {
                softwareUpdateManager.GetState(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error GetState");
            }
        }

        @Override
        public void onServiceDisconnected() {
            actionsFab.setEnabled(false);
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

    private void openSubMenusFab() {
        layoutFabAvailable.setVisibility(View.VISIBLE);
        layoutFabCommissioned.setVisibility(View.VISIBLE);
        fabExpanded = true;
    }

    private void closeSubMenusFab() {
        layoutFabAvailable.setVisibility(View.INVISIBLE);
        layoutFabCommissioned.setVisibility(View.INVISIBLE);
        fabExpanded = false;
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate");

        setContentView(R.layout.activity_main);
        recyclerView = (RecyclerView) findViewById(R.id.recycler_view);
        layoutFabAvailable = (LinearLayout) findViewById(R.id.getAvailable);
        layoutFabCommissioned = (LinearLayout) findViewById(R.id.getCommissioned);

        actionsFab = (FloatingActionButton) findViewById(R.id.actionFab);
        availableFab = (FloatingActionButton) findViewById(R.id.getAvailableFab);
        commissionedFab = (FloatingActionButton) findViewById(R.id.getCommissionedFab);

        swInfos = new ArrayList<SoftwareInformation>();
        adapter = new SoftwareInformationAdapter(this, swInfos, this);
        actionsFab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (fabExpanded)
                    closeSubMenusFab();
                else
                    openSubMenusFab();
            };
        });

        availableFab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    softwareUpdateManager.GetSoftwareAssignments();
                } catch (Exception e) {
                    Log.e(LOG_TAG, e.getMessage());
                }
            };
        });

	commissionedFab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    Log.v(LOG_TAG, "Sending GetPendingInstallations");
                    softwareUpdateManager.GetPendingInstallations();
                } catch (Exception e) {
                    Log.e(LOG_TAG, e.getMessage());
                }
            };
        });

        closeSubMenusFab();

        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, true);
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.addItemDecoration(new VerticalSpaceItemDecoration(10));
        recyclerView.setAdapter(adapter);
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
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);
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
}
