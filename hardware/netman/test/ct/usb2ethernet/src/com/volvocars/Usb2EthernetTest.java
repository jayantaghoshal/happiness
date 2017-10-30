package com.volvocars.usb2ethernet;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;
import android.net.ConnectivityManager;
import static org.junit.Assert.assertEquals;
import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;
import java.lang.String;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class Usb2EthernetTest extends
    ActivityTestRule<Usb2EthernetActivity>{

    @Rule
    public ActivityTestRule<Usb2EthernetActivity> mActivityRule =
            new ActivityTestRule<>(Usb2EthernetActivity.class);

    public Usb2EthernetTest(){
        super(Usb2EthernetActivity.class);
    }

    @Test
    public void testEthernetConnectionType() throws Exception{
        int TargetNetworkType = mActivityRule.getActivity().getNetworkType();
        assertEquals("NetworkType should be '9'(Ethernet) but is: " + 
            Integer.toString(TargetNetworkType), TargetNetworkType, 
            ConnectivityManager.TYPE_ETHERNET);
    }
}