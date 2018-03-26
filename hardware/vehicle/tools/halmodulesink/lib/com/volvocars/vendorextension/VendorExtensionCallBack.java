/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vendorextension;

import android.car.hardware.CarVendorExtensionManager;

public abstract class VendorExtensionCallBack implements CarVendorExtensionManager.CarVendorExtensionCallback {

    public int propId;
    public int areaId;

    public VendorExtensionCallBack(int propId, int areaId){
        this.propId = propId;
        this.areaId = areaId;
    }

}
