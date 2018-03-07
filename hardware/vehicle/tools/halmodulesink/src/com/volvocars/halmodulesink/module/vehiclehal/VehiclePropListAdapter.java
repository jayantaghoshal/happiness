/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module.vehiclehal;

import android.app.Activity;
import android.app.Dialog;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.Bundle;
import android.os.RemoteException;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.RecyclerView.ViewHolder;
import android.util.Log;
import android.util.Pair;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;
import com.volvocars.test.lib.vehiclehal.PropertyTimeoutException;
import com.volvocars.test.lib.vehiclehal.VehicleHal;
import com.volvocars.test.lib.vehiclehal.VehicleHalUtil;

import java.util.List;
import java.util.Vector;

public class VehiclePropListAdapter extends
        RecyclerView.Adapter<VehiclePropListAdapter.ProfileViewHolder> {
    public static final String TAG = VehiclePropListAdapter.class.getSimpleName();
    /**
     * Get information from HAL and update the list
     */
    public static final int GET_PROPERTY_RETRY = 3;
    private static final int LAYOUT_AREA_PADDING = 16;
    private static final int LAYOUT_BUTTON_PADDING = 5;
    private static final int textViewPadding = 5;
    private static final String BUTTON_SET_DATA = "More";
    VehiclePropListAdapter adapter;
    RecyclerView recyclerView;
    private Vector<VehiclePropConfig> vehiclePropData;
    private VehicleHal vehicleHal;
    private Activity context;

    public VehiclePropListAdapter(VehicleHal halTemp, RecyclerView recyclerView,
                                  AModuleFragment fragment) {
        vehiclePropData = new Vector<>();
        context = fragment.getActivity();
        vehicleHal = halTemp;
        adapter = this;
        this.recyclerView = recyclerView;
    }

    // Create new views (invoked by the layout manager)
    @Override
    public ProfileViewHolder onCreateViewHolder(ViewGroup parent,
                                                int viewType) {
        // create a new view
        View v = LayoutInflater.from(context)
                .inflate(R.layout.fragment_vehicle_hal_generic_cardview, parent, false);

        ProfileViewHolder vh = new ProfileViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(ProfileViewHolder holder, int position) {
        // Reuse the view
        if (holder != null) {
            final VehiclePropConfig vehiclePropConfig = vehiclePropData.get(position);
            holder.propIdTextView.setText(
                    VehicleHalUtil.getPropertyName(vehiclePropConfig.prop) +
                            " (" + vehiclePropConfig.prop + ")");
            // Removes dynamic views and reset layout
            holder.resetLayout();
            if ((vehiclePropConfig.prop & VehicleArea.MASK) != VehicleArea.GLOBAL) {
                Log.d(TAG, "Multi area handling:");

                // Multi areaID handling
                vehiclePropConfig.areaConfigs.forEach(vehicleAreaConfig -> {
                    LinearLayout infoLinearLayout = createLinearLayoutArea();
                    infoLinearLayout.addView(createTextView(
                            "AreaID (" + vehiclePropData.get(position).areaConfigs.size() + "): "
                                    + VehicleHalUtil.getVehicleZoneNameByVehicleAreaAndAreaId((vehiclePropConfig.prop & VehicleArea.MASK), vehicleAreaConfig.areaId)));

                    Button setDataButton = addButton(BUTTON_SET_DATA);


                    // Handle setting data
                    setDataButton.setOnClickListener(event -> {
                        try {
                            VehiclePropValue value = vehicleHal.get(
                                    vehiclePropConfig.prop,
                                    vehicleAreaConfig.areaId);
                            value.areaId = vehicleAreaConfig.areaId;
                            SetPropertyDialog cdd = new SetPropertyDialog(context, value, vehiclePropConfig);
                            cdd.show();
                        } catch (PropertyTimeoutException e) {
                            e.printStackTrace();
                        }

                    });
                    infoLinearLayout.addView(setDataButton);
                    ((LinearLayout) holder.baseLinearLayout.getParent()).addView(infoLinearLayout);
                });
            } else {
                Log.d(TAG, "Global areaID handling");
                Button setDataButton = addButton(BUTTON_SET_DATA);

                // Handle setting data
                setDataButton.setOnClickListener(event -> {
                    SetPropertyDialog cdd = null;
                    try {
                        cdd = new SetPropertyDialog(context,
                                vehicleHal.get(vehiclePropConfig.prop), vehiclePropConfig);
                        cdd.show();
                    } catch (PropertyTimeoutException e) {
                        e.printStackTrace();
                    }
                });

                holder.baseLinearLayout.addView(setDataButton);
            }
        } else {
            Log.w(TAG, "Holder of the recycle view is null");
        }
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return vehiclePropData.size();
    }

    public void addDataList(List<VehiclePropConfig> items) {
        Log.d(TAG, "addDataList is called. " + items);
        vehiclePropData.clear();
        vehiclePropData.addAll(items);
        updateUI();
    }

    public void changeItem() {
        vehiclePropData.clear();
        for (int i = 0; i < GET_PROPERTY_RETRY; i++) {
            try {
                vehiclePropData.addAll(vehicleHal.getAllPropConfigsDirect());
                Log.d(TAG, "ChangeItem:" + vehiclePropData);
                updateUI();
                return;
            } catch (RemoteException e) {
                Log.e(TAG, "Retrying to get vehicle prop items from HAL, tried: " + (i + 1));
            }
        }
    }

    /**
     * Update the UI with runOnUi thread
     */
    private void updateUI() {
        context.runOnUiThread(() -> {
            recyclerView.removeAllViews();
            adapter.notifyDataSetChanged();
            recyclerView.invalidate();
        });
    }


    /**
     * Gets value and type information from the raw data
     *
     * @return Pair of type info and value
     */
    private Pair<String, String> getTypeInfoFromRawData(VehiclePropValue vehiclePropValue) {
        String info = "";
        String value = "";
        if (!vehiclePropValue.value.int32Values.isEmpty()) {
            info = "int32Values";
            value = vehiclePropValue.value.int32Values.get(0).toString();
        } else if (!vehiclePropValue.value.floatValues.isEmpty()) {
            info = "floatValues";
            value = vehiclePropValue.value.floatValues.get(0).toString();
        } else if (!vehiclePropValue.value.int64Values.isEmpty()) {
            info = "int64Values";
            value = vehiclePropValue.value.int64Values.get(0).toString();
        } else if (!vehiclePropValue.value.bytes.isEmpty()) {
            info = "bytes";
            value = vehiclePropValue.value.bytes.get(0).toString();
        } else if (!vehiclePropValue.value.stringValue.isEmpty()) {
            info = "stringValue";
            value = vehiclePropValue.value.stringValue;
        } else {
            info = "nothing";
        }
        return new Pair(info, value);
    }

    private LinearLayout createLinearLayoutArea() {
        LinearLayout linearLayout = new LinearLayout(context);
        LinearLayout.LayoutParams layoutForInner = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        linearLayout.setLayoutParams(layoutForInner);
        linearLayout.setOrientation(LinearLayout.HORIZONTAL);
        linearLayout.setPadding(LAYOUT_AREA_PADDING, LAYOUT_AREA_PADDING, LAYOUT_AREA_PADDING,
                LAYOUT_AREA_PADDING);
        return linearLayout;
    }

    private LinearLayout createLinearLayoutButton() {
        LinearLayout linearLayout = new LinearLayout(context);
        LinearLayout.LayoutParams layoutForInner = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layoutForInner.weight = 1.0f;
        layoutForInner.gravity = Gravity.CENTER_HORIZONTAL;
        linearLayout.setLayoutParams(layoutForInner);
        linearLayout.setOrientation(LinearLayout.HORIZONTAL);
        linearLayout.setPadding(LAYOUT_BUTTON_PADDING, LAYOUT_BUTTON_PADDING, LAYOUT_BUTTON_PADDING,
                LAYOUT_BUTTON_PADDING);
        return linearLayout;
    }

    private TextView createTextView(String text) {
        TextView textView = new TextView(context);
        LinearLayout.LayoutParams
                layoutForInner = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.MATCH_PARENT);
        textView.setLayoutParams(layoutForInner);
        textView.setPadding(textViewPadding, textViewPadding, textViewPadding, textViewPadding);
        textView.setText(text);
        textView.setTextSize(context.getResources().getDimension(R.dimen.vehiclehal_recyclerview_textsize));
        return textView;
    }

    private Button addButton(String text) {
        Button button = new Button(context);
        LinearLayout.LayoutParams param = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.MATCH_PARENT);
        button.setLayoutParams(param);
        button.setText(text);
        button.setTextAppearance(context, R.style.buttonStyle);
        return button;
    }

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder
    public static class ProfileViewHolder extends ViewHolder {
        private TextView propIdTextView = null;
        private LinearLayout baseLinearLayout = null;
        private int childCount;
        private int parentCount;

        public ProfileViewHolder(View itemView) {
            super(itemView);
            baseLinearLayout = (LinearLayout) itemView.findViewById(R.id.base_linear_layout);
            propIdTextView = (TextView) itemView.findViewById(R.id.propid_textview);
            childCount = baseLinearLayout.getChildCount();
            parentCount = ((LinearLayout)baseLinearLayout.getParent()).getChildCount();

        }

        public void resetLayout() {
            while (baseLinearLayout.getChildCount() > childCount) {
                baseLinearLayout.removeViewAt(baseLinearLayout.getChildCount() - 1);
            }
            LinearLayout parent = (LinearLayout)baseLinearLayout.getParent();
            while (parent.getChildCount() > parentCount) {
                parent.removeViewAt(parent.getChildCount() - 1);
            }
        }
    }


    /**
     * Custom Dialog box for setting propValue
     */
    public class SetPropertyDialog extends Dialog implements
            android.view.View.OnClickListener {

        public Activity activity;
        public Dialog dialog;
        public Button closeButton;
        public Button changeButton;
        public EditText editText;
        public TextView infoTypeText;
        public VehiclePropValue vehiclePropValue;
        public TextView headText;
        public String stringValue = new String();
        public VehiclePropConfig vehiclePropConfig;

        public SetPropertyDialog(Activity activity, VehiclePropValue vehiclePropValue, VehiclePropConfig vehiclePropConfig) {
            super(activity);
            // TODO Auto-generated constructor stub
            this.activity = activity;
            this.vehiclePropValue = vehiclePropValue;
            this.vehiclePropConfig = vehiclePropConfig;
        }

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            requestWindowFeature(Window.FEATURE_NO_TITLE);
            setContentView(R.layout.fragment_vehicle_hal_generic_setdata);
            final Pair typeInfo = getTypeInfoFromRawData(vehiclePropValue);


            headText = (TextView) findViewById(R.id.textInfoType);
            closeButton = (Button) findViewById(R.id.HalGenericButtonClose);
            changeButton = (Button) findViewById(R.id.HalGenericButtonChange);

            editText = (EditText) findViewById(R.id.editTextValue);
            infoTypeText = (TextView) findViewById(R.id.textViewType);

            infoTypeText.setText(typeInfo.first.toString());
            editText.setText(typeInfo.second.toString());

            // Header information
            StringBuilder header = new StringBuilder();
            try {
                header
                        .append("General info of VehiclePropConfig:")
                        .append(System.getProperty("line.separator"))
                        .append(vehiclePropConfig.toString())
                        .append(System.getProperty("line.separator"))
                        .append(System.getProperty("line.separator"))
                        .append(System.getProperty("line.separator"))
                        .append("General info of VehiclePropValue:")
                        .append(System.getProperty("line.separator"))
                        .append(vehiclePropValue.toString())
                        .append(System.getProperty("line.separator"));
            } catch (Exception e) {
                Log.e(TAG, "Error for getting information from VehicleHal.", e);
                header.append(e.getMessage());
            }
            headText.setText(header);

            closeButton.setOnClickListener(event -> {
                dismiss();
            });

            changeButton.setOnClickListener(event -> {
                boolean result = true;
                try {

                    // Handle type of the setting a value
                    switch (typeInfo.first.toString()) {
                        case "int32Values":
                            vehiclePropValue.value.int32Values.clear();
                            vehiclePropValue.value.int32Values.add(
                                    Integer.valueOf(editText.getText().toString()));
                            vehicleHal.set(vehiclePropValue);
                            break;
                        case "floatValues":
                            vehiclePropValue.value.floatValues.clear();
                            vehiclePropValue.value.floatValues.add(
                                    Float.valueOf(editText.getText().toString()));
                            vehicleHal.set(vehiclePropValue);
                            break;
                        case "int64Values":
                            vehiclePropValue.value.int64Values.clear();
                            vehiclePropValue.value.int64Values.add(
                                    Long.valueOf(editText.getText().toString()));
                            vehicleHal.set(vehiclePropValue);
                            break;
                        case "bytes":
                            vehiclePropValue.value.bytes.clear();
                            vehiclePropValue.value.bytes.add(
                                    Byte.valueOf(editText.getText().toString()));
                            vehicleHal.set(vehiclePropValue);
                            break;
                        case "stringValue":
                            vehiclePropValue.value.stringValue = editText.getText().toString();
                            vehicleHal.set(vehiclePropValue);
                            break;
                        case "nothing":
                            result = false;
                            break;
                    }
                } catch (Exception e) {
                    result = false;
                    Log.e(TAG, "Cannot set a value", e);
                }
                if (result) {
                    changeItem();
                    dismiss();
                }
            });
        }

        @Override
        public void onClick(View view) {
            dismiss();
        }
    }
}

