/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.content.Context;
import android.graphics.Color;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.ScrollView;
import android.widget.TextView;
import android.util.Log;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.SoftwareInformation;

import java.util.ArrayList;

public class SoftwareInformationAdapter extends RecyclerView.Adapter<SoftwareInformationAdapter.MyViewHolder> {
    private Context context;
    private ArrayList<SoftwareInformation> swInfos;
    private ISoftwareUpdateApp softwareUpdateApp;

    public static class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView swId, name, desc, installationId, installationStatus, state, downloads, downloadedResources;
        public LinearLayout swIdLayout, installationIdLayout, installationStatusLayout, downloadsLayout,
                downloadedResourcesLayout;
        public ImageView overflow, icon;
        public CardView cardView;
        private boolean moreInfo = false;
        private boolean showInstallNotifcationItem = false;

        public MyViewHolder(View view) {
            super(view);
            swId = (TextView) view.findViewById(R.id.swId);
            swIdLayout = (LinearLayout) view.findViewById(R.id.swIdLayout);
            name = (TextView) view.findViewById(R.id.name);
            desc = (TextView) view.findViewById(R.id.desc);
            installationId = (TextView) view.findViewById(R.id.installationId);
            installationIdLayout = (LinearLayout) view.findViewById(R.id.installationIdLayout);
            downloads = (TextView) view.findViewById(R.id.downloads);
            downloadsLayout = (LinearLayout) view.findViewById(R.id.downloadsLayout);
            downloadedResourcesLayout = (LinearLayout) view.findViewById(R.id.downloadedResourcesLayout);
            downloadedResources = (TextView) view.findViewById(R.id.downloadedResources);
            installationStatus = (TextView) view.findViewById(R.id.installationStatus);
            installationStatusLayout = (LinearLayout) view.findViewById(R.id.installationStatusLayout);
            state = (TextView) view.findViewById(R.id.state);
            overflow = (ImageView) view.findViewById(R.id.overflow);
            icon = (ImageView) view.findViewById(R.id.icon);
            cardView = (CardView) view.findViewById(R.id.assignmentCV);
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
    private void showPopupMenu(MyViewHolder holder, SoftwareInformation sw) {
        // inflate menu
        PopupMenu popup = new PopupMenu(context, holder.overflow);
        MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.menu_assignment, popup.getMenu());
        popup.setOnMenuItemClickListener(new MyMenuItemClickListener(holder, sw));
        if (!holder.state.getText().equals(SoftwareAssignment.Status.COMMISSIONABLE.name()))
            popup.getMenu().findItem(R.id.commission).setVisible(false);
        if (holder.state.getText().equals("INSTALL PENDING"))
            holder.showInstallNotifcationItem = true;

        popup.getMenu().findItem(R.id.installNotification).setVisible(holder.showInstallNotifcationItem);
        popup.show();
    }

    /**
     * Click listener for popup menu items
     */
    class MyMenuItemClickListener implements PopupMenu.OnMenuItemClickListener {
        private SoftwareInformation swInfo;
        private MyViewHolder holder;

        public MyMenuItemClickListener(MyViewHolder holder, SoftwareInformation swInfo) {
            this.holder = holder;
            this.swInfo = swInfo;
        }

        @Override
        public boolean onMenuItemClick(MenuItem menuItem) {
            switch (menuItem.getItemId()) {
            case R.id.commission:
                softwareUpdateApp.commissionAssignment(swInfo.softwareAssignment.id);
                return true;
            case R.id.installNotification:
                softwareUpdateApp.getInstallNotification(swInfo.softwareAssignment.installationOrder.id);
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

        holder.name.setText(swInfo.softwareAssignment.name);
        holder.desc.setText(swInfo.softwareAssignment.shortDescription);
        holder.state.setText(((swInfo.softwareState == SoftwareInformation.SoftwareState.UNDEFINED) ? swInfo.softwareAssignment.status.name() : swInfo.softwareState.name()).replace("_", " "));

        holder.swId.setText(swInfo.softwareAssignment.id);
        if (holder.moreInfo)
            holder.swIdLayout.setVisibility(View.VISIBLE);
        else
            holder.swIdLayout.setVisibility(View.INVISIBLE);

        holder.installationId.setText(swInfo.softwareAssignment.installationOrder.id);
        if (holder.moreInfo && !swInfo.softwareAssignment.installationOrder.id.isEmpty())
            holder.installationIdLayout.setVisibility(View.VISIBLE);
        else
            holder.installationIdLayout.setVisibility(View.INVISIBLE);

        holder.installationStatus.setText(swInfo.softwareAssignment.installationOrder.status.name());
        if (holder.moreInfo && !swInfo.softwareAssignment.installationOrder.status.name().isEmpty())
            holder.installationStatusLayout.setVisibility(View.VISIBLE);
        else
            holder.installationStatusLayout.setVisibility(View.INVISIBLE);

        String str = "";
        if(!swInfo.downloadInfo.resourceUris.isEmpty()) {
            str = Integer.toString(swInfo.downloadInfo.resourceUris.size());
        }
        holder.downloads.setText(str);
        if (holder.moreInfo && !swInfo.downloadInfo.resourceUris.isEmpty())
            holder.downloadsLayout.setVisibility(View.VISIBLE);
        else
            holder.downloadsLayout.setVisibility(View.INVISIBLE);

        str = "";
        if(!swInfo.downloadInfo.downloadedResources.isEmpty()) {
            str = Integer.toString(swInfo.downloadInfo.downloadedResources.size());
        }

        holder.downloadedResources.setText(str);
        if (holder.moreInfo && !swInfo.downloadInfo.downloadedResources.isEmpty())
            holder.downloadedResourcesLayout.setVisibility(View.VISIBLE);
        else
            holder.downloadedResourcesLayout.setVisibility(View.INVISIBLE);
        holder.overflow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showPopupMenu(holder, swInfos.get(position));
            }
        });

        holder.cardView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                holder.moreInfo = !holder.moreInfo;
                if (holder.moreInfo) {
                    holder.swIdLayout.setVisibility(View.VISIBLE);
                    if (!swInfo.softwareAssignment.installationOrder.id.isEmpty())
                        holder.installationIdLayout.setVisibility(View.VISIBLE);
                    if (!swInfo.softwareAssignment.installationOrder.status.name().isEmpty())
                        holder.installationStatusLayout.setVisibility(View.VISIBLE);
                    if (!swInfo.downloadInfo.resourceUris.isEmpty())
                        holder.downloadsLayout.setVisibility(View.VISIBLE);
                    if (!swInfo.downloadInfo.downloadedResources.isEmpty())
                        holder.downloadedResourcesLayout.setVisibility(View.VISIBLE);
                } else {
                    holder.swIdLayout.setVisibility(View.INVISIBLE);
                    holder.installationIdLayout.setVisibility(View.INVISIBLE);
                    holder.installationStatusLayout.setVisibility(View.INVISIBLE);
                    holder.downloadsLayout.setVisibility(View.INVISIBLE);
                    holder.downloadedResourcesLayout.setVisibility(View.INVISIBLE);
                }
            }
        });

        if (swInfo.softwareAssignment.deliverableType == SoftwareAssignment.DeliverableType.NEW) {
            holder.icon.setImageResource(R.drawable.outline_add_black_24dp);
        }
        else if (swInfo.softwareAssignment.deliverableType == SoftwareAssignment.DeliverableType.UPDATE) {
            holder.icon.setImageResource(R.drawable.ic_system_update_black_24dp);
        }

    }

    @Override
    public int getItemCount() {
        return swInfos.size();
    }
}