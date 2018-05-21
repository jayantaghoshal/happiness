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

import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.Notification;
import com.volvocars.cloudservice.Status;
import com.volvocars.cloudservice.parser.XmlParser;

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class InstallNotificationParser extends XmlParser {

    public static InstallNotification ParseInstallNotification(InputStream in) throws XmlPullParserException, IOException {

        Log.v(LOG_TAG, "+ ParseInstallNotification");

        InstallNotification installNotification = new InstallNotification();

        XmlPullParser parser = Xml.newPullParser();
        parser.setInput(in, null);

        int eventType = parser.getEventType();
        while (eventType != XmlPullParser.END_DOCUMENT) {
            if (eventType == XmlPullParser.START_DOCUMENT) {
                Log.v(LOG_TAG, "Start of Document");
            } else if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("install_notifications")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installNotification = ParseInstallNotificationElement(parser);
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

        return installNotification;
    }

    private static InstallNotification ParseInstallNotificationElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("install_notifications"))) {
            throw new IllegalStateException();
        }

        InstallNotification installNotification = new InstallNotification();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("this")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("installation_order_id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installNotification.installationOrderId = ParseString("installation_order_id", parser);
                } else if (tag.equals("software_id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installNotification.softwareId = ParseString("software_id", parser);
                } else if (tag.equals("notification")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installNotification.notification = ParseNotificationElement(parser);
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("install_notifications")) {
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
        return installNotification;
    }

    private static Notification ParseNotificationElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("notification"))) {
            throw new IllegalStateException();
        }
        Notification notification = new Notification();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    notification.id = ParseString(tag, parser);
                } else if (tag.equals("created")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    notification.created = ParseString(tag, parser);
                } else if (tag.equals("timestamp")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    notification.timestamp = ParseString(tag, parser);
                } else if (tag.equals("status_code")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    notification.status.statusCode = Status.stringToStatusCode(ParseString(tag, parser));
                } else if (tag.equals("sub_status_code")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    notification.status.subStatusCode = Status.stringToSubStatusCode(ParseString(tag, parser));
                }else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("notification")) {
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
        return notification;
    }
}