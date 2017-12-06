package com.volvocars.powerevent;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

/**
 * A sample alert UI
 */
public class AlertActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alert);

        findViewById(R.id.button).setOnClickListener(buttonClickListener);
    }

    private View.OnClickListener buttonClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            AlertActivity.this.finish();
        }
    };
}