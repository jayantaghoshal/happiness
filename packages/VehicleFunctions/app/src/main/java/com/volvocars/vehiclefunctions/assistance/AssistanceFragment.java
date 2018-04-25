/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.volvocars.vehiclefunctions.R;

public class AssistanceFragment extends Fragment {

    private AssistanceViewModel viewModel;

    public static AssistanceFragment instantiate() {
        return new AssistanceFragment();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        AssistanceViewModelFactory factory = new AssistanceViewModelFactory(getActivity().getApplication());
        viewModel = factory.get(this);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_assistance, container, false);

        final FunctionsAdapter functionsAdapter = new FunctionsAdapter(getLayoutInflater(),
                viewModel.getSections());

        final GridLayoutManager gridLayoutManager = new GridLayoutManager(getActivity(),
                getResources().getInteger(R.integer.assistance_column_count));
        gridLayoutManager.setSpanSizeLookup(new GridLayoutManager.SpanSizeLookup() {
            @Override
            public int getSpanSize(int position) {
                return functionsAdapter.getSpanSize(position);
            }
        });

        final RecyclerView recyclerView = view.findViewById(R.id.recyclerView);
        recyclerView.setHasFixedSize(true);
        recyclerView.setAdapter(functionsAdapter);
        recyclerView.setLayoutManager(gridLayoutManager);

        return view;
    }

}
