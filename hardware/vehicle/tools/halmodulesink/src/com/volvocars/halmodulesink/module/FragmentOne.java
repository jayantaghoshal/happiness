/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Template for creating a new model
 *
 */
public class FragmentOne extends AModuleFragment {

    public static final String TAG = FragmentOne.class.getSimpleName();
    public static final String logCatregex = "logcat -d com.volvocars.halmodulesink:ERROR *:S";
    public static final String TITLE = "Example";
    private volatile TextView terminalLog;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_one, null);
        terminalLog = (TextView) root.findViewById(R.id.log);
        printLogCat();
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        getActivity().setTitle(TITLE);
    }

    /**
     * Print out log cat
     */
    public void printLogCat() {
        try {
            Process process = Runtime.getRuntime().exec(logCatregex);
            BufferedReader bufferedReader = new BufferedReader(
                    new InputStreamReader(process.getInputStream()));
            StringBuilder log = new StringBuilder();
            log.append("Log Cat:\n");
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                log.append(line);
            }
            updateUI(() -> {
                terminalLog.append(log);
            });
        } catch (IOException e) {
        }
    }

}