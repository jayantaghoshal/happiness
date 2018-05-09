/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package android.volvocars.localconfig;

/**
 * Exception thrown when Local Config Service is not connected for the API which requires Service connection.
 */
public class LcfgNotConnectedException extends Exception {
    private static final long serialVersionUID = 42112L;
    public LcfgNotConnectedException() {
    }
    public LcfgNotConnectedException(String name) {
        super(name);
    }
    public LcfgNotConnectedException(String name, Throwable cause) {
        super(name, cause);
    }
    public LcfgNotConnectedException(Exception cause) {
        super(cause);
    }
}