/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task;

import java.text.MessageFormat;

import android.os.AsyncTask;

public abstract class BasePropertyAsyncTask extends AsyncTask<Void, Void, Void> {
    protected String BASE_LOG_TAG;
    protected int mRequestIdentifier;

    public BasePropertyAsyncTask(String parentLogTag, int requestIdentifier) {
        BASE_LOG_TAG = parentLogTag;
        mRequestIdentifier = requestIdentifier;
    }

    protected String getLogTag() {
        return MessageFormat.format(BASE_LOG_TAG + ".", this.getClass().getName());
    }
}
