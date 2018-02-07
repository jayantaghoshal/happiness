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
import com.volvocars.cloudservice.SoftwareAssignment;
import java.util.ArrayList;

public class AssignmentAdapter extends RecyclerView.Adapter<AssignmentAdapter.MyViewHolder> {
    private Context context;
    private ArrayList<SoftwareAssignment> assignments;
    private ISoftwareUpdateApp softwareUpdateApp;

    public static class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView name, desc;
        public ImageView commissionImage;

        public MyViewHolder(View view) {
            super(view);
            name = (TextView) view.findViewById(R.id.name);
            desc = (TextView) view.findViewById(R.id.description);
            commissionImage = (ImageView) view.findViewById(R.id.commissionImage);
        }
    }

    public AssignmentAdapter(Context context, ArrayList<SoftwareAssignment> assignments,
            ISoftwareUpdateApp softwareUpdateApp) {
        this.context = context;
        this.assignments = assignments;
        this.softwareUpdateApp = softwareUpdateApp;
    }

    /**
     * Showing popup menu when tapping on 3 dots
     */
    private void showPopupMenu(View view, SoftwareAssignment sw) {
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
        private SoftwareAssignment sw;

        public MyMenuItemClickListener(SoftwareAssignment sw) {
            this.sw = sw;
        }

        @Override
        public boolean onMenuItemClick(MenuItem menuItem) {
            switch (menuItem.getItemId()) {
            case R.id.commission:
                softwareUpdateApp.commissionAssignment(sw.uuid);
                Toast.makeText(context, "Commissioning \"" + sw.name + "\"", Toast.LENGTH_SHORT).show();
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
        SoftwareAssignment sw = assignments.get(position);
        holder.name.setText(sw.name);
        holder.desc.setText(sw.description);

        holder.commissionImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showPopupMenu(holder.commissionImage, assignments.get(position));
            }
        });
    }

    @Override
    public int getItemCount() {
        return assignments.size();
    }
}