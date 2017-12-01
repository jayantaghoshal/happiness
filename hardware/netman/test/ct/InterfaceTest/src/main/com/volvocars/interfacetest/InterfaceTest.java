package com.volvocars.networkinterfacetest;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;
import android.net.ConnectivityManager;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;
import java.lang.String;
import java.util.ArrayList;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class InterfaceTest extends
    ActivityTestRule<MainActivity>{

    @Rule
    public ActivityTestRule<MainActivity> mActivityRule =
            new ActivityTestRule<>(MainActivity.class);

    public InterfaceTest(){
        super(MainActivity.class);
    }

    @Test
    public void testInterface() throws Exception{
        ArrayList<String> ifaceList = mActivityRule.getActivity().checkNetworkInterfaces();

        // Only lo and sit0 should be visible
        // Might need to add eth0 or similar later
        assertEquals(ifaceList.size(), 2);
        assertTrue(ifaceList.contains("lo"));
        assertTrue(ifaceList.contains("sit0"));
    }
}
