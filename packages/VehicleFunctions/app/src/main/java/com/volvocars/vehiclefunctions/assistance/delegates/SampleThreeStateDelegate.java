/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunction;

/*
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiState().
 * Dependencies should be injected.
 *
 * This class can be removed when real functionality is implemented.
 */
public class SampleThreeStateDelegate extends ThreeStateFunction.Delegate {

    public SampleThreeStateDelegate(/* Vehicle function API objects */) {
    }

    @Override
    protected void onUserChangedState(int state) {
        // Use API to communicate with vehicle and then call setUiState() when ready
        setUiState(state);
    }

    @Override
    protected void onSetInitialState() {
        // Use API to communicate with vehicle and then call setUiState() when ready
        setUiState(0);
        setUiEnabled(true);
    }
}
