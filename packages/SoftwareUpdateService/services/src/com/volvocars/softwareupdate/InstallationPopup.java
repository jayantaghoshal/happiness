/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;

import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;

import android.os.Bundle;
import android.util.Log;

import android.view.WindowManager;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;

/**
 * Class to show installation pop-up as a system alert dialog
 * (Used for testing, might want to redo/delete this once common popup framework is in place)
 */
public class InstallationPopup extends Activity {
    private static final String LOG_TAG = "SoftwareUpdate.Popup";
    private Context context = this;
    private Messenger messageHandler;
    private String name;
    private String uuid;

    public enum InstallOption {
        INSTALL, CANCEL, POSTPONE
    }

    public static final String NAME = "name";
    public static final String UUID = "uuid";
    public static final String OPTION = "option";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.w(LOG_TAG, "InstallationPopup started, this is a temporary solution...");
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate()");
        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        name = intent.getStringExtra(NAME);
        uuid = intent.getStringExtra(UUID);
        messageHandler = (Messenger) bundle.get("MESSENGER");

        showInstallPopup();
    }

    private void showInstallPopup() {
        Log.v(LOG_TAG, "showInstallPopup");

        AlertDialog.Builder builder = new AlertDialog.Builder(this)
        .setTitle("Software Update")
        .setMessage(name + " is ready to be installed. Install now?");
        AlertDialog dialog = builder.create();
        dialog.setButton(AlertDialog.BUTTON_POSITIVE, "Install", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                Log.v(LOG_TAG, "Install button pressed");
                sendMessage(InstallOption.INSTALL);
                ((Activity) context).finish();
            }
        });
        dialog.setButton(AlertDialog.BUTTON_NEGATIVE, "Cancel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                Log.v(LOG_TAG, "Cancel button pressed");
                sendMessage(InstallOption.CANCEL);
                ((Activity) context).finish();
            }
        });
        dialog.setButton(AlertDialog.BUTTON_NEUTRAL, "Postpone", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                Log.v(LOG_TAG, "Postpone button pressed");
                sendMessage(InstallOption.POSTPONE);
                ((Activity) context).finish();
            }
        });
        dialog.setOnCancelListener(new DialogInterface.OnCancelListener() {

            public void onCancel(DialogInterface dialog) {
                ((Activity) context).finish();
            }
        });
        dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        dialog.show();
    }

    private void sendMessage(InstallOption option) {
        Message message = Message.obtain();
        Bundle bundle = new Bundle();
        bundle.putString(UUID, uuid);
        bundle.putSerializable(OPTION, option);
        message.setData(bundle);
        try {
            Log.v(LOG_TAG, "sending message...");
            messageHandler.send(message);
        } catch (RemoteException e) {
            Log.v(LOG_TAG, "Can't send the message: RemoteException [ " + e.getMessage() + "]");
        }
    }
}