/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package jsqlite;

/**
 * Class for SQLite related exceptions.
 */

public class Exception extends java.lang.Exception {

    /**
     * Construct a new SQLite exception.
     *
     * @param string error message
     */

    public Exception(String string) {
	super(string);
    }
}
