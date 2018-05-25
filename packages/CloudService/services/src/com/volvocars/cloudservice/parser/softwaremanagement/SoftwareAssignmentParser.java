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

import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.parser.XmlParser;

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class SoftwareAssignmentParser extends XmlParser {

    private static ArrayList<SoftwareAssignment> ParseAvailableAccessoriesElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || !(parser.getName().equals("available_accessories"))) {
            throw new IllegalStateException();
        }

        ArrayList<SoftwareAssignment> list = new ArrayList();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("this")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("software")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    list.add(ParseSoftwareElement(parser));
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("available_accessories")) {
                    depth--;
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Unknown End of Tag: " + parser.getName());
                }
            } else if (eventType == XmlPullParser.END_DOCUMENT) {
                Log.e(LOG_TAG, LOG_PREFIX + " Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
                throw new IllegalStateException();
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Unknown element on depth: " + depth);
            }
        }

        return list;
    }

    public static ArrayList<SoftwareAssignment> ParseSoftwareAssignments(InputStream in)
    throws XmlPullParserException, IOException {

    Log.v(LOG_TAG, LOG_PREFIX + " + ParseSoftwareAssignments");

    ArrayList<SoftwareAssignment> list = new ArrayList();

    XmlPullParser parser = Xml.newPullParser();
    parser.setInput(in, null);

    int eventType = parser.getEventType();
    while (eventType != XmlPullParser.END_DOCUMENT) {
        if (eventType == XmlPullParser.START_DOCUMENT) {
            Log.v(LOG_TAG, LOG_PREFIX + " Start of Document");
        } else if (eventType == XmlPullParser.START_TAG) {
            String tag = parser.getName();
            if (tag.equals("available_updates")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                list = ParseAvailableUpdatesElement(parser);
                Log.w(LOG_TAG, LOG_PREFIX + " Parse: " + list.size());
            }
            else if (tag.equals("available_accessories")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                list = ParseAvailableAccessoriesElement(parser);
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

    Log.v(LOG_TAG, LOG_PREFIX + " - ParseSoftwareAssignments");

    return list;
    }

    private static ArrayList<SoftwareAssignment> ParseAvailableUpdatesElement(XmlPullParser parser)
        throws XmlPullParserException, IOException {
    int eventType = parser.getEventType();
    if ((eventType != XmlPullParser.START_TAG) || !(parser.getName().equals("available_updates"))) {
        throw new IllegalStateException();
    }

    ArrayList<SoftwareAssignment> list = new ArrayList();

    int depth = 1;
    while (depth != 0) {
        eventType = parser.next();
        if (eventType == XmlPullParser.START_TAG) {
            String tag = parser.getName();
            if (tag.equals("this")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                SkipElement(parser);
            } else if (tag.equals("software")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                list.add(ParseSoftwareElement(parser));
            } else if (tag.equals("Signature")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                SkipElement(parser);
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Skipping unknown tag: " + tag);
                SkipElement(parser);
            }
        } else if (eventType == XmlPullParser.END_TAG) {
            if (parser.getName().equals("available_updates")) {
                depth--;
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Unknown End of Tag: " + parser.getName());
            }
        } else if (eventType == XmlPullParser.END_DOCUMENT) {
            Log.e(LOG_TAG, LOG_PREFIX + " Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
            throw new IllegalStateException();
        } else {
            Log.d(LOG_TAG, LOG_PREFIX + " Unknown element on depth: " + depth);
        }
    }

    return list;
    }

    private static SoftwareAssignment ParseSoftwareElement(XmlPullParser parser)
        throws XmlPullParserException, IOException {
    int eventType = parser.getEventType();
    if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("software"))) {
        throw new IllegalStateException();
    }
    SoftwareAssignment software = new SoftwareAssignment();
    Log.w(LOG_TAG, LOG_PREFIX + " ParseSoftwareElement called");

    int depth = 1;
    while (depth != 0) {
        eventType = parser.next();
        if (eventType == XmlPullParser.START_TAG) {
            String tag = parser.getName();
            if (tag.equals("id")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.id = ParseString(tag, parser);
            } else if (tag.equals("name")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.name = ParseString(tag, parser);
            } else if (tag.equals("icon")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                SkipElement(parser);
            } else if (tag.equals("short_description")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.shortDescription = ParseString(tag, parser);
            } else if (tag.equals("long_description")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                SkipElement(parser); //Todo: parseLongDescription
            } else if (tag.equals("size")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Skipping: " + tag);
                software.size = Double.valueOf(ParseString(tag, parser));
            } else if (tag.equals("software_product_id")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.softwareProductId = ParseString(tag, parser);
            } else if (tag.equals("software_product_version")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.softwareProductVersion = ParseString(tag, parser);
            } else if (tag.equals("status")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.status = SoftwareAssignment.stringToStatus(ParseString(tag, parser));
            } else if (tag.equals("installation_order")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.installationOrder = ParseInstallationOrderElement(parser);
            } else if (tag.equals("type")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.type = SoftwareAssignment.stringToType(ParseString(tag, parser));
            } else if (tag.equals("deliverable_type")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.deliverableType = SoftwareAssignment.stringToDeliverableType(ParseString(tag, parser));
            } else if (tag.equals("installation_type")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.installationType = SoftwareAssignment.stringToInstallationType(ParseString(tag, parser));
            } else if (tag.equals("estimated_installation_time")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.estimatedInstallationTime = Integer.valueOf(ParseString(tag, parser));
            } else if (tag.equals("commission_uri")) {
                Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                software.commissionUri = ParseString(tag, parser);
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Skipping unknown tag: " + tag);
                SkipElement(parser);
            }
        } else if (eventType == XmlPullParser.END_TAG) {
            if (parser.getName().equals("software")) {
                depth--;
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Unknown End of Tag: " + parser.getName());
            }
        } else if (eventType == XmlPullParser.END_DOCUMENT) {
            Log.e(LOG_TAG, LOG_PREFIX + " Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
            throw new IllegalStateException();
        } else {
            Log.d(LOG_TAG, LOG_PREFIX + " Unknown element on depth: " + depth);
        }
    }
    return software;
    }

    private static InstallationOrder ParseInstallationOrderElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("installation_order"))) {
            throw new IllegalStateException();
        }
        InstallationOrder installationOrder = new InstallationOrder();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("id")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    installationOrder.id = ParseString(tag, parser);
                } else if (tag.equals("downloads_uri")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    installationOrder.downloadsUri = ParseString(tag, parser);
                } else if (tag.equals("install_notifications_uri")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    installationOrder.installNotificationsUri = ParseString(tag, parser);
                } else if (tag.equals("installation_report_uri")) {
                    Log.v(LOG_TAG, LOG_PREFIX + " Parsing: " + tag);
                    installationOrder.installationReportUri = ParseString(tag, parser);
                }  else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("installation_order")) {
                    depth--;
                } else {
                    Log.d(LOG_TAG, LOG_PREFIX + " Unknown End of Tag: " + parser.getName());
                }
            } else if (eventType == XmlPullParser.END_DOCUMENT) {
                Log.e(LOG_TAG, LOG_PREFIX + " Oh gosh, darnit! Unecpected end of document. Maybe throw some stuff?");
                throw new IllegalStateException();
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " Unknown element on depth: " + depth);
            }
        }
        return installationOrder;
    }

}