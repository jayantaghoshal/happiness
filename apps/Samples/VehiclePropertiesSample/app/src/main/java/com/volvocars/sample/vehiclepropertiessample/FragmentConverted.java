/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.sample.vehiclepropertiessample;

import android.content.Context;
import android.os.Bundle;

import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import com.volvocars.carconfig.CarConfigEnums;
import com.volvocars.carconfig.CarConfigApi;



/**
 * MainActivityTest
 */
public class FragmentConverted extends Fragment {
    private static Context mContext;
    private static final String TAG = FragmentConverted.class.getSimpleName();
    private StringBuilder text = new StringBuilder();
    private EditText testTextView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_translate_data, container, false);
        mContext = getContext();
        Button testButton = rootView.findViewById(R.id.buttonGetProperties);
        testTextView = rootView.findViewById(R.id.testLog);

        // Check if no view has
        testButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Using CarConfig and CarConfig API\n\n");
                text.append(CarConfigEnums.CC_1_VehicleType.class.getSimpleName() + "->");
                text.append(CarConfigApi.getValue(CarConfigEnums.CC_1_VehicleType.class)+"\n");
                text.append(CarConfigEnums.CC_3_TransmissionDriveline.class.getSimpleName() + "->");
                text.append(CarConfigApi.getValue(CarConfigEnums.CC_3_TransmissionDriveline.class) +"\n");
                text.append(CarConfigEnums.CC_4_ElectricDriveline.class.getSimpleName() + "->");
                text.append(CarConfigApi.getValue(CarConfigEnums.CC_4_ElectricDriveline.class) +"\n\n");
                text.append("Checking CC_4_ElectricDriveline with switch: " +"\n");

                switch (CarConfigApi.getValue(CarConfigEnums.CC_4_ElectricDriveline.class)) {
                    case Without_Power_Flow:
                        text.append("Without_Power_Flow");
                        break;
                    case Combustion_Engine_FWD_and_Electric_Motor_FWD:
                        text.append("Combustion_Engine_FWD_and_Electric_Motor_FWD");
                        break;
                    case Combustion_Engine_FWD_and_Electric_Motor_RWD:
                        text.append("Combustion_Engine_FWD_and_Electric_Motor_RWD");
                        break;
                    case Combustion_Engine_RWD_and_Electric_Motor_FWD:
                        text.append("Combustion_Engine_RWD_and_Electric_Motor_FWD");
                        break;
                    case Combustion_Engine_RWD_and_Electric_Motor_RWD:
                        text.append("Combustion_Engine_RWD_and_Electric_Motor_RWD");
                        break;
                    case Electric_Motor_AWD:
                        text.append("Electric_Motor_AWD");
                        break;
                    case Electric_Motor_FWD:
                        text.append("Electric_Motor_FWD");
                        break;
                    case Electric_Motor_RWD:
                        text.append("Electric_Motor_RWD");
                        break;
                    default: text.append("\n Nothing found!");
                }

                text.append("Convertion done!");
                testTextView.setText(text);
            }
        });
        return rootView;
    }
}
