/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package jsqlite;

/**
 * Callback interface for SQLite's profile function.
 */

public interface Profile {

    /**
     * Callback to profile (ie log) one SQL statement
     * with its estimated execution time.
     *
     * @param stmt SQL statement string
     * @param est  estimated execution time in milliseconds.
     */

    public void profile(String stmt, long est);
}

