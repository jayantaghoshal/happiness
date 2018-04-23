/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

import android.support.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertThat;

import com.android.tradefed.testtype.HostTest;

@RunWith(AndroidJUnit4.class)
@SmallTest
public class TestRemoteClimateService extends HostTest {

    @Test
    public void TestMe() {
        assertThat(true, true);
    }
}
