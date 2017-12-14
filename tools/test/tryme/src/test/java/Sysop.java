/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

import static org.testng.Assert.assertNotNull;

/**
 * Class Sysop, for "System Operator"
 *
 * Intended to provide access functions
 * for a SUT (System Under Test) to
 * other running test code.
 *
 * @param systemSpec A System Specification that uniquely
 *                  identifies the node for the Sysop to
 *                  connect to and operate.
 * @return systemHandle An interface to the system,
 *                      such as command interfaces, logs and
 *                      connection status.
 */
public class Sysop {
    private String systemSpec;

    public Sysop(String systemSpec) throws InterruptedException {
        assertNotNull(systemSpec);
        this.systemSpec = systemSpec;

        System.out.println("Sysop: Performing init");

        // Thread for handling link to SUT over ADB
        Thread linkAdb = new Thread(new LinkAdbThread());
        System.out.println("Sysop: Starting ADB thread " + linkAdb.getName());
        linkAdb.start();
        while (linkAdb.isAlive()) {
            linkAdb.join(5000);
            linkAdb.interrupt();
            linkAdb.join();
        }
    }
}

