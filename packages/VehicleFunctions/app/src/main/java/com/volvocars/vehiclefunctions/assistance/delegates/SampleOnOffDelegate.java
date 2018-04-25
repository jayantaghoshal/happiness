/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunction;

/*
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiState().
 * Dependencies should be injected.
 *
 * This class can be removed when real functionality is implemented.
 */
public class SampleOnOffDelegate extends OnOffFunction.Delegate {

    public SampleOnOffDelegate(/* Vehicle function API objects */) {
    }

    @Override
    protected void onUserChangedState(boolean value) {
        // Use API to communicate with vehicle and then call setUiState() when ready
        setUiState(value);
    }

    @Override
    protected void onSetInitialState() {
        // Use API to communicate with vehicle and then call setUiState() when ready
        setUiState(true);
        setUiEnabled(true);
    }
}
