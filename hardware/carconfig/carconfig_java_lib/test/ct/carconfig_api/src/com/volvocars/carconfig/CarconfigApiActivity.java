/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.carconfig;

import android.app.Activity;
import android.os.Bundle;
import com.volvocars.carconfig.CarConfigEnums;
import com.volvocars.carconfig.CarConfigApi;

public class CarconfigApiActivity extends Activity{

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public void test(){}

    /**
     * Get a car value from the car configuration
     * @return
     */
    public CarConfigEnums.CC_1_VehicleType getCC1Value(){
        return CarConfigApi.getValue(CarConfigEnums.CC_1_VehicleType.class);
    }

    /**
     * Get a null
     * @return
     */
    public CarConfigEnums.CC_1_VehicleType getNull(){
        return (CarConfigApi.getValue(null));
    }
}