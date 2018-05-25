/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice.parser.softwaremanagement;

import java.io.IOException;
import java.io.StringReader;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;
import android.util.Log;
import android.util.Xml;

import com.volvocars.cloudservice.SoftwareManagementURIs;
import com.volvocars.cloudservice.parser.XmlParser;
import com.volvocars.cloudservice.SoftwareManagementURIs;

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class SoftwareManagementParser extends XmlParser {

    public static SoftwareManagementURIs ParseSoftwareManagementURIs(InputStream in)
            throws XmlPullParserException, IOException {

        Log.v(LOG_TAG, LOG_PREFIX + " + ParseSoftwareManagementURIs");

        SoftwareManagementURIs uris = new SoftwareManagementURIs("", "", "", "");

        XmlPullParser parser = Xml.newPullParser();
        parser.setInput(in, null);

        int eventType = parser.getEventType();
        while (eventType != XmlPullParser.END_DOCUMENT) {
            if (eventType == XmlPullParser.START_DOCUMENT) {
                Log.v(LOG_TAG, LOG_PREFIX + " Start of Document");
            } else if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("software_management")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    uris = ParseSoftwareManagementElement(parser);
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Skipping Unknown Tag: " + tag);
                    SkipElement(parser);
                }
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Unknown Element of type: " + eventType);
            }
            eventType = parser.next();
        }
        Log.v(LOG_TAG, LOG_PREFIX + " End of Document");

        Log.v(LOG_TAG, LOG_PREFIX + " - ParseSoftwareManagementURIs");
        return uris;
    }

    private static SoftwareManagementURIs ParseSoftwareManagementElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("software_management"))) {
            throw new IllegalStateException();
        }

        SoftwareManagementURIs uris = new SoftwareManagementURIs("", "", "", "");

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("this")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("available_updates")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    uris.available_updates = ParseString("available_updates", parser);
                } else if (tag.equals("available_accessories")) {
                    Log.v(LOG_TAG, " [SoftwareManagementParser]Parsing: " + tag);
                    uris.available_accessories = ParseString("available_accessories", parser);
                } else if (tag.equals("map_updates")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("downloads")) {
                    Log.v(LOG_TAG, " [SoftwareManagementParser]Parsing: " + tag);
                    uris.downloads = ParseString("downloads", parser);
                } else if (tag.equals("delete_notification")) {
                    Log.v(LOG_TAG, " [SoftwareManagementParser]Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("closed_installations")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("software_management")) {
                    depth--;
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + "Unknown End of Tag: " + parser.getName());
                }
            } else if (eventType == XmlPullParser.END_DOCUMENT) {
                Log.e(LOG_TAG, LOG_PREFIX + " Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
                throw new IllegalStateException();
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + "Unknown element on depth: " + depth);
            }
        }
        return uris;
    }


}