/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

/**
 * A sample alert UI and will be removed later
 */
public class RemoveMeAlertActivity extends Activity {
    private View.OnClickListener buttonClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            RemoveMeAlertActivity.this.finish();
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alert);

        TextView action = findViewById(R.id.action);
        TextView info = findViewById(R.id.info);

        action.setText(getIntent().getExtras().getString("action"));
        String infoS = getIntent().getExtras().getString("info");
        if (infoS != null) {
            info.setText(infoS);
        }
        findViewById(R.id.button).setOnClickListener(buttonClickListener);
    }
}