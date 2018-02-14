/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.PopupMenu;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import android.util.Log;
import com.volvocars.softwareupdate.SoftwareInformation;

import java.util.ArrayList;

public class SoftwareInformationAdapter extends RecyclerView.Adapter<SoftwareInformationAdapter.MyViewHolder> {
    private Context context;
    private ArrayList<SoftwareInformation> swInfos;
    private ISoftwareUpdateApp softwareUpdateApp;

    public static class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView swId, name, desc, installationId, installationStatus, state, downloads;
        public ImageView overflow;

        public MyViewHolder(View view) {
            super(view);
            swId = (TextView) view.findViewById(R.id.swId);
            name = (TextView) view.findViewById(R.id.name);
            desc = (TextView) view.findViewById(R.id.desc);
            installationId = (TextView) view.findViewById(R.id.installationId);
            downloads = (TextView) view.findViewById(R.id.downloads);
            installationStatus = (TextView) view.findViewById(R.id.installationStatus);
            state = (TextView) view.findViewById(R.id.state);
            downloads = (TextView) view.findViewById(R.id.downloads);
            overflow = (ImageView) view.findViewById(R.id.overflow);
        }
    }

    public SoftwareInformationAdapter(Context context, ArrayList<SoftwareInformation> swInfos,
            ISoftwareUpdateApp softwareUpdateApp) {
        this.context = context;
        this.swInfos = swInfos;
        this.softwareUpdateApp = softwareUpdateApp;
    }

    /**
     * Showing popup menu when tapping on 3 dots
     */
    private void showPopupMenu(View view, SoftwareInformation sw) {
        // inflate menu
        PopupMenu popup = new PopupMenu(context, view);
        MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.menu_assignment, popup.getMenu());
        popup.setOnMenuItemClickListener(new MyMenuItemClickListener(sw));
        popup.show();
    }

    /**
     * Click listener for popup menu items
     */
    class MyMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {
        private SoftwareInformation swInfo;

        public MyMenuItemClickListener(SoftwareInformation swInfo) {
            this.swInfo = swInfo;
        }

        @Override
        public boolean onMenuItemClick(MenuItem menuItem) {
            switch (menuItem.getItemId()) {
            case R.id.commission:
                softwareUpdateApp.commissionAssignment(swInfo.softwareId);
                Toast.makeText(context, "Commissioning \"" + swInfo.name + "\"", Toast.LENGTH_SHORT).show();
                return true;
            default:
            }
            return false;
        }
    }

    @Override
    public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.assignment_card, parent, false);
        return new MyViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(final MyViewHolder holder, int position) {
        SoftwareInformation swInfo = swInfos.get(position);
        if (!swInfo.name.isEmpty())
            holder.name.setText(swInfo.name);
        if (!swInfo.softwareId.isEmpty())
            holder.swId.setText(swInfo.softwareId);
        if (!swInfo.description.isEmpty())
            holder.desc.setText(swInfo.description);
        if (!swInfo.installationId.isEmpty())
            holder.installationId.setText(swInfo.installationId);
        if (!swInfo.installationStatus.isEmpty())
            holder.installationStatus.setText(swInfo.installationStatus);
        if (!swInfo.softwareState.name().isEmpty())
            holder.state.setText(swInfo.softwareState.name());

            String str = "";
        for (String download : swInfo.downloads) {
            str += download + "\n";
        }
        if (!str.isEmpty())
            holder.downloads.setText(str);

        holder.overflow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showPopupMenu(holder.overflow, swInfos.get(position));
            }
        });
    }

    @Override
    public int getItemCount() {
        return swInfos.size();
    }
}