/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions;

import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;

import com.volvocars.hmi.uxlibrary.application.BottomNavigationBar;

public class VehicleFunctionsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vehicle_functions);

        BottomNavigationBar bottomNavigationBar = findViewById(R.id.bottomNavigationBar);
        CustomViewPager viewPager = findViewById(R.id.vehicleFunctionsViewPager);
        viewPager.setAdapter(new VehicleFunctionsPagerAdapter(this));

        bottomNavigationBar.setOnNavigationItemSelectedListener(item -> {
            int position = 0;

            switch (item.getItemId()) {
                case R.id.driving_item:
                    position = 0;
                    break;
                case R.id.assistance_item:
                    position = 1;
                    break;
                case R.id.status_items:
                    position = 2;
                    break;
            }

            bottomNavigationBar.setCurrentIndex(position);
            viewPager.setCurrentItem(position, false);
            return true;
        });

        viewPager.setPagingEnabled(false);
    }
}
