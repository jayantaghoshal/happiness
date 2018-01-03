/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.microphonetestapp;

import android.content.Context;
import android.media.AudioManager;
import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    AudioManager audioManager = null;
    boolean isMicMuted = false;

    public AudioManager getAudioManager() {return audioManager;}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        audioManager = (AudioManager)
                getSystemService(Context.AUDIO_SERVICE);
    }
}
