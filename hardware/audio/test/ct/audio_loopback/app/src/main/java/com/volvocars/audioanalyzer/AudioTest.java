/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.audioanalyzer;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;
import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;
import static org.junit.Assert.assertTrue;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class AudioTest extends
    ActivityTestRule<MainActivity>{

    @Rule
    public ActivityTestRule<MainActivity> mActivityRule =
            new ActivityTestRule<>(MainActivity.class);

    public AudioTest(){
        super(MainActivity.class);
    }

    @Test
    public void testAudio() throws Exception{
        mActivityRule.getActivity().playSound(200);

        assertTrue(mActivityRule.getActivity().getCompareFreqs());
    }
}

