package com.volvocars.sample.vehiclepropertiessample;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


public class FragmentHome extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_home, container, false);
        String readMe = "This is a demo/sample application for using CarConfig Java API.\n";
        TextView textView = rootView.findViewById(R.id.textExplain);
        textView.setLines(textView.getMaxLines());
        textView.setText(readMe);
        return rootView;
    }

}
