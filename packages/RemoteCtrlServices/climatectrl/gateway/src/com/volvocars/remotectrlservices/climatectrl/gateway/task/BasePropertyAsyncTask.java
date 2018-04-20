/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import android.os.AsyncTask;

public abstract class BasePropertyAsyncTask extends AsyncTask<Void, Void, Void> {
    protected int mRequestIdentifier;

    public BasePropertyAsyncTask(int requestIdentifier) {
        mRequestIdentifier = requestIdentifier;
    }
}
