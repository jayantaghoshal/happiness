/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

import org.testng.annotations.Test;
import java.lang.String;

import static org.testng.Assert.assertEquals;

public class AICTest {
    @Test
    public void testAdd() {
        String str = "First TestNG output";
        assertEquals("First TestNG output", str);

        try {
            Sysop testSysop = new Sysop("BenchMk1");
        } catch (InterruptedException eInt) {
            System.out.println("Sysop: Interrupted while waiting for thread");
            System.exit(1);
        }
    }
}