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

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.parser.XmlParser;

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class DownloadInfoParser extends XmlParser {

    public static DownloadInfo ParseDownloadInfo(InputStream in) throws XmlPullParserException, IOException {

        Log.v(LOG_TAG, "+ ParseDownloadInfo");

        DownloadInfo downloadInfo = new DownloadInfo();

        XmlPullParser parser = Xml.newPullParser();
        parser.setInput(in, null);

        int eventType = parser.getEventType();
        while (eventType != XmlPullParser.END_DOCUMENT) {
            if (eventType == XmlPullParser.START_DOCUMENT) {
                Log.v(LOG_TAG, "Start of Document");
            } else if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("downloads")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloadInfo = ParseDownloadInfoElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping Unknown Tag: " + tag);
                    SkipElement(parser);
                }
            } else {
                Log.d(LOG_TAG, "Unknown Element of type: " + eventType);
            }
            eventType = parser.next();
        }
        Log.v(LOG_TAG, "End of Document");

        Log.v(LOG_TAG, "- ParseDownloadInfo");

        return downloadInfo;
    }

    private static DownloadInfo ParseDownloadInfoElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("downloads"))) {
            throw new IllegalStateException();
        }

        DownloadInfo downloadInfo = new DownloadInfo();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("this")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloadInfo.id = ParseString("id", parser);
                } else if (tag.equals("install_notification_uri")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloadInfo.installNotificationUri = ParseString("install_notification_uri", parser);
                } else if (tag.equals("installation_order_id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloadInfo.installationOrderId = ParseString("installation_order_id", parser);
                } else if (tag.equals("downloads")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloadInfo.resourceUris = ParseDownloadPaths(parser);
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("downloads")) {
                    depth--;
                } else {
                    Log.d(LOG_TAG, "Unknown End of Tag: " + parser.getName());
                }
            } else if (eventType == XmlPullParser.END_DOCUMENT) {
                Log.e(LOG_TAG, "Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
                throw new IllegalStateException();
            } else {
                Log.d(LOG_TAG, "Unknown element on depth: " + depth);
            }
        }
        return downloadInfo;
    }

    private static ArrayList<String> ParseDownloadPaths(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("downloads"))) {
            throw new IllegalStateException();
        }

        ArrayList<String> downloads = new ArrayList<String>();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("file")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    downloads.add(ParseString("file", parser));
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("downloads")) {
                    depth--;
                } else {
                    Log.d(LOG_TAG, "Unknown End of Tag: " + parser.getName());
                }
            } else if (eventType == XmlPullParser.END_DOCUMENT) {
                Log.e(LOG_TAG, "Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
                throw new IllegalStateException();
            } else {
                Log.d(LOG_TAG, "Unknown element on depth: " + depth);
            }
        }
        return downloads;
    }
}
