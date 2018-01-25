/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.Bundle;
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

import java.util.List;
import java.util.Vector;

public class MyAdapter extends RecyclerView.Adapter<MyAdapter.ProfileViewHolder> {
    public static final String TAG = MyAdapter.class.getSimpleName();
    private static final int LAYOUT_AREA_PADDING = 16;
    private static final int LAYOUT_BUTTON_PADDING = 5;
    private static final int textViewPadding = 5;
    private static final String BUTTON_MORE_INFO = "Get more information";
    private static final String BUTTON_SET_DATA = "Set Data";
    MyAdapter adapter;
    RecyclerView recyclerView;
    private Vector<VehiclePropConfig> mDataset;
    private VehicleHal mHal;
    private Activity context;
    private AModuleFragment fragment;
    private int PropId;


    public MyAdapter(VehicleHal halTemp, RecyclerView recyclerView, AModuleFragment fragment) {
        this.fragment = fragment;
        mDataset = new Vector<>();
        context = fragment.getActivity();
        mHal = halTemp;
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
            holder.propIdTextView.setText(mDataset.get(position).prop + "");
            // Removes dynamic views and reset layout
            holder.resetLayout();
            if (mDataset.get(position).areaConfigs != null && !mDataset.get(
                    position).areaConfigs.isEmpty()) {
                Log.d(TAG, "Multi area handling:");
                // Showing information about access for the multi areaID
                LinearLayout accessLinearLayout = createLinearLayoutArea();
                accessLinearLayout.addView(createTextView("Access information:"));
                accessLinearLayout.addView(createTextView(mDataset.get(position).toString()));
                holder.baseLinearLayout.addView(accessLinearLayout);

                // Multi areaID handling
                mDataset.get(position).areaConfigs.forEach(vehicleAreaConfig -> {
                    LinearLayout infoLinearLayout = createLinearLayoutArea();
                    infoLinearLayout.addView(createTextView(
                            "AreaID (" + mDataset.get(position).areaConfigs.size() + "): "
                                    + String.valueOf(vehicleAreaConfig.areaId)));
                    infoLinearLayout.addView(createTextView("Data:"));
                    infoLinearLayout.addView(createTextView(vehicleAreaConfig.toString()));
                    holder.baseLinearLayout.addView(infoLinearLayout);
                    LinearLayout buttonLayout = createLinearLayoutButton();
                    Button moreInfoButton = addButton(BUTTON_MORE_INFO);
                    Button setDataButton = addButton(BUTTON_SET_DATA);

                    // Handle more information button
                    moreInfoButton.setOnClickListener(event -> {
                        StringBuilder result = new StringBuilder();
                        try {
                            VehiclePropValue value = mHal.get(mDataset.get(position).prop,
                                    vehicleAreaConfig.areaId);
                            result.append("Value (" + getTypeInfoFromRawData(value).first + "): ")
                                    .append(getTypeInfoFromRawData(value).second)
                                    .append(System.getProperty("line.separator"))
                                    .append(System.getProperty("line.separator"))
                                    .append("RawData info:")
                                    .append(System.getProperty("line.separator"))
                                    .append(value.toString());
                        } catch (Exception e) {
                            Log.e(TAG, "Error for getting information from VehicleHal.", e);
                            result.append(e.getMessage());
                        }
                        AlertDialog alertDialog = new AlertDialog.Builder(context).create();
                        alertDialog.setTitle("Getting areaID: " + vehicleAreaConfig.areaId
                                + " and result.");
                        alertDialog.setMessage(result);
                        alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK",
                                new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int which) {
                                        dialog.dismiss();
                                    }
                                });
                        alertDialog.show();
                    });

                    // Handle setting data
                    setDataButton.setOnClickListener(event -> {
                        try {
                            VehiclePropValue value = mHal.get(mDataset.get(position).prop,
                                    vehicleAreaConfig.areaId);
                            value.areaId = vehicleAreaConfig.areaId;
                            CustomDialogClass cdd = new CustomDialogClass(context, value);
                            cdd.show();
                        } catch (PropertyTimeoutException e) {
                            e.printStackTrace();
                        }

                    });
                    buttonLayout.addView(moreInfoButton);
                    buttonLayout.addView(setDataButton);
                    holder.baseLinearLayout.addView(buttonLayout);
                });
            } else {
                Log.d(TAG, "Global areaID handling");
                // Global areaID handling
                LinearLayout infoLinearLayout = createLinearLayoutArea();
                infoLinearLayout.addView(createTextView("No area found, Data:"));
                infoLinearLayout.addView(createTextView(mDataset.get(position).toString()));
                holder.baseLinearLayout.addView(infoLinearLayout);
                LinearLayout buttonLayout = createLinearLayoutButton();
                Button moreInfoButton = addButton(BUTTON_MORE_INFO);
                Button setDataButton = addButton(BUTTON_SET_DATA);

                // Handle more information button
                moreInfoButton.setOnClickListener(event -> {
                    StringBuilder result = new StringBuilder();
                    try {
                        VehiclePropValue value = mHal.get(mDataset.get(position).prop);
                        result.append("Value (" + getTypeInfoFromRawData(value).first + "): ")
                                .append(getTypeInfoFromRawData(value).second)
                                .append(System.getProperty("line.separator"))
                                .append(System.getProperty("line.separator"))
                                .append("RawData info:")
                                .append(System.getProperty("line.separator"))
                                .append(value.toString());
                    } catch (Exception e) {
                        Log.e(TAG, "Error for getting information from VehicleHal.", e);
                        result.append(e.getMessage());
                    }

                    AlertDialog alertDialog = new AlertDialog.Builder(context).create();
                    alertDialog.setTitle(
                            "Getting Global areaID with PropID: " + mDataset.get(position).prop);
                    alertDialog.setMessage(result);
                    alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK",
                            new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.dismiss();
                                }
                            });
                    alertDialog.show();
                });


                // Handle setting data
                setDataButton.setOnClickListener(event -> {
                    CustomDialogClass cdd = null;
                    try {
                        cdd = new CustomDialogClass(context, mHal.get(mDataset.get(position).prop));
                        cdd.show();
                    } catch (PropertyTimeoutException e) {
                        e.printStackTrace();
                    }
                });

                buttonLayout.addView(moreInfoButton);
                buttonLayout.addView(setDataButton);
                holder.baseLinearLayout.addView(buttonLayout);
            }
        }
    }

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return mDataset.size();
    }

    public void addDataList(List<VehiclePropConfig> items) {
        Log.d(TAG, "addDataList is called. " + items);
        mDataset.clear();
        mDataset.addAll(items);
        updateUI();
    }

    /**
     * Get information from HAL and update the list
     */
    public void changeItem() {
        mDataset.clear();
        mDataset.addAll(mHal.getAllPropConfigsDirect());
        Log.d(TAG, "ChangeItem:" + mDataset);
        updateUI();
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
        linearLayout.setOrientation(LinearLayout.VERTICAL);
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

        public ProfileViewHolder(View itemView) {
            super(itemView);
            baseLinearLayout = (LinearLayout) itemView.findViewById(R.id.base_linear_layout);
            propIdTextView = (TextView) itemView.findViewById(R.id.propid_textview);
            childCount = baseLinearLayout.getChildCount();

        }

        public void resetLayout() {
            while (baseLinearLayout.getChildCount() > childCount) {
                baseLinearLayout.removeViewAt(1);
            }
        }
    }


    /**
     * Custom Dialog box for setting propValue
     */
    public class CustomDialogClass extends Dialog implements
            android.view.View.OnClickListener {

        public Activity activity;
        public Dialog dialog;
        public Button closeButton;
        public Button changeButton;
        public EditText editText;
        public TextView infoTypeText;
        public VehiclePropValue vehiclePropValue;
        public String stringValue = new String();

        public CustomDialogClass(Activity activity, VehiclePropValue vehiclePropValue) {
            super(activity);
            // TODO Auto-generated constructor stub
            this.activity = activity;
            this.vehiclePropValue = vehiclePropValue;
        }

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            requestWindowFeature(Window.FEATURE_NO_TITLE);
            setContentView(R.layout.fragment_vehicle_hal_generic_setdata);
            final Pair typeInfo = getTypeInfoFromRawData(vehiclePropValue);


            closeButton = (Button) findViewById(R.id.HalGenericButtonClose);
            changeButton = (Button) findViewById(R.id.HalGenericButtonChange);

            editText = (EditText) findViewById(R.id.editTextValue);
            infoTypeText = (TextView) findViewById(R.id.textViewType);

            infoTypeText.setText(typeInfo.first.toString());
            editText.setText(typeInfo.second.toString());

            closeButton.setOnClickListener(event -> {
                dismiss();
            });

            changeButton.setOnClickListener(event -> {
                boolean result = true;
                try {

                    // Handle type of the setting a value
                    switch (typeInfo.first.toString()) {
                        case "int32Values":
                            vehiclePropValue.value.int32Values.remove(
                                    vehiclePropValue.value.int32Values.get(0));
                            vehiclePropValue.value.int32Values.add(
                                    Integer.valueOf(editText.getText().toString()));
                            mHal.set(vehiclePropValue);
                            break;
                        case "floatValues":
                            vehiclePropValue.value.floatValues.remove(
                                    vehiclePropValue.value.floatValues.get(0));
                            vehiclePropValue.value.floatValues.add(
                                    Float.valueOf(editText.getText().toString()));
                            mHal.set(vehiclePropValue);
                            break;
                        case "int64Values":
                            vehiclePropValue.value.int64Values.remove(
                                    vehiclePropValue.value.int64Values.get(0));
                            vehiclePropValue.value.int64Values.add(
                                    Long.valueOf(editText.getText().toString()));
                            mHal.set(vehiclePropValue);
                            break;
                        case "bytes":
                            vehiclePropValue.value.bytes.remove(
                                    vehiclePropValue.value.bytes.get(0));
                            vehiclePropValue.value.bytes.add(
                                    Byte.valueOf(editText.getText().toString()));
                            mHal.set(vehiclePropValue);
                            break;
                        case "stringValue":
                            vehiclePropValue.value.stringValue = "";
                            vehiclePropValue.value.stringValue = editText.getText().toString();
                            mHal.set(vehiclePropValue);
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

