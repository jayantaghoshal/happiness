/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.carconfig;

import android.util.Log;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Permission handling need to be investigated
 */

public class CarConfigApi{

    public static final String TAG = CarConfigApi.class.getSimpleName();
    public static final String CAR_CONFIG_LOCATION = "/oem_config/carconfig/carconfig.csv";
    public static final String CAR_CONFIG_LOCATION_DEFAULT = "/vendor/etc/config/carconfig-default.csv";
    private static ArrayList<Integer> carConfigData = new ArrayList<>();

    /**
     * Allow only static access and prevent the class from being instantiated
     */
    private CarConfigApi(){
    }

    /**
     * Get a value from the volvo car config by given enum
     * @param carConfigEnum
     * @return Enum
     */
    @SuppressWarnings("unchecked")
    public static synchronized <T extends CarConfigEnums.CarConfigEnumBase> T getValue(Class<T> carConfigEnum){
        if (carConfigData.size()==0){
            loadData();
        }
        int param = CarConfigEnums.getParamNumber(carConfigEnum);
        return (T) CarConfigEnums.getValue(carConfigEnum, carConfigData.get(param-1));
    }

    /**
     * Load data by reading the volvo CarConfig file
     */
    private static void loadData(){
        File volvoCarConfig = new File(CAR_CONFIG_LOCATION);

        // If the file doesn't exist, get the default version
        if (!volvoCarConfig.exists()){
            volvoCarConfig = new File(CAR_CONFIG_LOCATION_DEFAULT);
        }
        String regexSplit = ",";
        String line = "";

        try (BufferedReader br = new BufferedReader(new FileReader(volvoCarConfig))){
            while ((line = br.readLine()) != null) {
                String[] arrayResult = line.split(regexSplit);
                if (arrayResult.length == 3){
                    carConfigData.add(Integer.valueOf(arrayResult[1]));
                } else {
                    Log.e(TAG, "Corrupted Volvo Car config ");
                }
            }
        } catch (FileNotFoundException e) {
            Log.e(TAG, "Volvo Car config not found", e);
        } catch (IOException e) {
            Log.e(TAG, "Reading volvo Car config has some issues", e);
        }
    }
}