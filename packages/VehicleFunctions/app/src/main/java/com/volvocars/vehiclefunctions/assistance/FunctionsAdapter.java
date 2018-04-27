/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance;

import android.arch.lifecycle.LiveData;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.volvocars.vehiclefunctions.R;
import com.volvocars.vehiclefunctions.assistance.functions.Function;
import com.volvocars.vehiclefunctions.assistance.functions.FunctionViewHolder;
import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunctionViewHolder;
import com.volvocars.vehiclefunctions.assistance.functions.OneButtonFunctionViewHolder;
import com.volvocars.vehiclefunctions.assistance.functions.Section;
import com.volvocars.vehiclefunctions.assistance.functions.SectionViewHolder;
import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunctionViewHolder;
import com.volvocars.vehiclefunctions.assistance.functions.IntegerFunctionViewHolder;
import java.util.ArrayList;
import java.util.List;

/**
 * RecyclerView.Adapter for displaying a grid of vehicle function cells, separated by their
 * parent sections.
 */
public class FunctionsAdapter extends RecyclerView.Adapter<FunctionViewHolder> {
    private final LayoutInflater mLayoutInflater;

    private final List<Function> mFunctionsFlatList = new ArrayList<>();

    public FunctionsAdapter(LayoutInflater layoutInflater, LiveData<List<Section>> sections) {
        mLayoutInflater = layoutInflater;
        sections.observeForever(newSections -> {
            setSections(newSections);
        });
    }

    private void setSections(List<Section> sections)
    {
        mFunctionsFlatList.clear();
        //Flatten the Sections/Functions into a single list for use by adapter.
        for (Section section : sections) {
            List<Function> sectionFunctions = section.getFunctions();
            if (!sectionFunctions.isEmpty()) {
                mFunctionsFlatList.add(section);
                mFunctionsFlatList.addAll(sectionFunctions);
            }
        }
        notifyDataSetChanged();
    }


    @Override
    public int getItemCount() {
        return mFunctionsFlatList.size();
    }

    @Override
    public int getItemViewType(int position) {
        return mFunctionsFlatList.get(position).getLayoutId();
    }

    @Override
    public FunctionViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = mLayoutInflater.inflate(viewType, parent, false);

        FunctionViewHolder viewHolder;

        switch (viewType) {
            case R.layout.vh_function_section:
                viewHolder = new SectionViewHolder(view);
                break;
            case R.layout.vh_function_onoff:
                viewHolder = new OnOffFunctionViewHolder(view);
                break;
            case R.layout.vh_function_threestate:
                viewHolder = new ThreeStateFunctionViewHolder(view);
                break;
            case R.layout.vh_function_onebutton:
                viewHolder = new OneButtonFunctionViewHolder(view);
                break;
            case R.layout.vh_function_plusminus:
                viewHolder = new IntegerFunctionViewHolder(view);
                break;
            default:
                viewHolder = new FunctionViewHolder(view);
                break;
        }

        return viewHolder;
    }

    @Override
    public void onBindViewHolder(FunctionViewHolder holder, int position) {
        Function function = mFunctionsFlatList.get(position);
        holder.bind(function);
    }

    /**
     * Returns the number of columns occupied by particular item.
     * Can be used to implement a GridLayoutManager.SpanSizeLookup delegate.
     *
     * @param position Position of the item in the adapter
     * @return The number of columns occupied by the item at the given position.
     */
    public int getSpanSize(int position) {
        return mFunctionsFlatList.get(position).getSlotsOccupied();
    }
}