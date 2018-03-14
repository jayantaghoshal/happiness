/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package jsqlite;

/**
 * Callback interface for SQLite's trace function.
 */

public interface Trace {

    /**
     * Callback to trace (ie log) one SQL statement.
     *
     * @param stmt SQL statement string
     */

    public void trace(String stmt);
}

