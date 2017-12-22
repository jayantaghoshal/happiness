package com.volvocars.microphonetestapp;

import android.media.AudioManager;
import android.os.SystemClock;
import android.support.test.filters.LargeTest;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;

import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

@RunWith(AndroidJUnit4.class)
@LargeTest
public class MicControlTest extends
    ActivityTestRule<MainActivity>{

    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule<MainActivity>(MainActivity.class);

    public MicControlTest(){
        super(MainActivity.class);
    }

    @Test
    public void muteMicrophone() throws Exception {
        AudioManager mAudioManager = mActivityRule.getActivity().getAudioManager();

        SystemClock.sleep(2000);
        //mute microphone
        mAudioManager.setMicrophoneMute(true);
        assertTrue(mAudioManager.isMicrophoneMute());

        SystemClock.sleep(4000);
        //unmute microphone
        mAudioManager.setMicrophoneMute(false);
        assertFalse(mAudioManager.isMicrophoneMute());

    }
}
