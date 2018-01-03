/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.carconfig;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;
import com.volvocars.carconfig.CarConfigEnums;
import com.volvocars.carconfig.CarConfigApi;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class CarconfigApiTest extends
    ActivityTestRule<CarconfigApiActivity>{

    @Rule
    public ActivityTestRule<CarconfigApiActivity> mActivityRule =
            new ActivityTestRule<>(CarconfigApiActivity.class);

    public CarconfigApiTest(){
        super(CarconfigApiActivity.class);
    }

    @Test
    public void testGetValueNotNull() throws Exception{
        Object carconfigCC1 = mActivityRule.getActivity().getCC1Value();
        assertNotNull("It should have some value ", carconfigCC1);
        assertTrue(carconfigCC1 instanceof CarConfigEnums.CC_1_VehicleType);
    }

    @Test
    public void testGetValueNullClass() throws Exception{
        CarConfigEnums.CC_1_VehicleType carconfigCC1 = mActivityRule.getActivity().getNull();
        assertTrue("Value should be null",carconfigCC1 == null);
    }
}