/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

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

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class XmlParser {
    private static final String LOG_TAG = "CloudService.XmlParser";
    private static final String null_string = null;

    public static ArrayList<Feature> parse(InputStream in) throws XmlPullParserException, IOException {
        try {
            XmlPullParser parser = Xml.newPullParser();
            parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
            parser.setInput(in, null);
            parser.nextTag();
            return readFeatures(parser);
        } finally {
            in.close();
        }
    }

    /**
    * Parses root elmeent which should be features, skips other tags.
    */
    private static ArrayList<Feature> readFeatures(XmlPullParser parser) throws XmlPullParserException, IOException {
        ArrayList<Feature> features = new ArrayList<Feature>();

        parser.require(XmlPullParser.START_TAG, null_string, "features");
        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String name = parser.getName();
            // Starts by looking for the feature tag
            if (name.equals("feature")) {
                features.add(readFeature(parser));
            } else {
                SkipElement(parser);
            }
        }
        return features;
    }

    /**
    /* Parses the contents of an entry. If it encounters other elements like name, enabled, visible,uri or tags hands them off
    /* to their respective "read" methods for processing. Otherwise, skips the tag.
    /* @param parser XmlPullParser object containg the data
    /* @return a Feature object.
     */
    private static Feature readFeature(XmlPullParser parser) throws XmlPullParserException, IOException {
        Feature feature = new Feature();

        parser.require(XmlPullParser.START_TAG, null_string, "feature");

        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String attributeName = parser.getName();
            if (attributeName.equals("name")) {
                feature.name = readAttributeAsString(parser, "name");
            } else if (attributeName.equals("enabled")) {
                feature.enabled = readAttributeAsBool(parser, "enabled");
            } else if (attributeName.equals("visible")) {
                feature.visible = readAttributeAsBool(parser, "visible");
            } else if (attributeName.equals("uri")) {
                feature.uri = readAttributeAsString(parser, "uri");
            } else if (attributeName.equals("tags")) {
                //Todo implement tags
                SkipElement(parser);
            } else {
                SkipElement(parser);
            }
        }
        return feature;
    }

    /**
    /* Read attribut or (tag) as bool
    /* @param parser XmlPullParser object containg the data
    /* @param attr attribute or (tag) to look for.
    /* @return read attribute or (tag)
    */
    private static Boolean readAttributeAsBool(XmlPullParser parser, String attr)
            throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, null_string, attr);
        Boolean name = readAsBool(parser);
        parser.require(XmlPullParser.END_TAG, null_string, attr);
        return name;
    }

    /** Read attribute or (tag) as string
    /* @param parser XmlPullParser object containg the data
    /* @param attr attribute or (tag) to look for.
    /* @return read attribute or (tag)
    */
    private static String readAttributeAsString(XmlPullParser parser, String attr)
            throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, null_string, attr);
        String name = readAsString(parser);
        parser.require(XmlPullParser.END_TAG, null_string, attr);
        return name;
    }

    /**
    /* Helper function for readAttributeAsBool
    /* Gets the text from parser.
    /* @param parser XmlPullParser object containg the data
    /* @return bool representation of the attribute or tag
    */
    private static Boolean readAsBool(XmlPullParser parser) throws IOException, XmlPullParserException {
        String result = "";
        if (parser.next() == XmlPullParser.TEXT) {
            result = parser.getText();
            parser.nextTag();
        }
        if (result.toLowerCase().equals("true"))
            return true;
        else
            return false;
    }

    /**
    /* Helper function for readAttributeAsString
    /* Gets the text from parser.
    /* @param parser XmlPullParser object containg the data
    /* @return string with content of the attribute or tag
    */
    private static String readAsString(XmlPullParser parser) throws IOException, XmlPullParserException {
        String result = "";
        if (parser.next() == XmlPullParser.TEXT) {
            result = parser.getText();
            parser.nextTag();
        }
        return result;
    }

    /*
     * ###########################################################################################
     * #                            PARSER FOR SOFTWARE ASSIGNMENT                                #
     * ###########################################################################################
     */

    public static SoftwareManagementURIs ParseSoftwareManagementURIs(InputStream in)
            throws XmlPullParserException, IOException {

        Log.v(LOG_TAG, "+ ParseSoftwareManagementURIs");

        SoftwareManagementURIs uris = new SoftwareManagementURIs("", "", "","");

        XmlPullParser parser = Xml.newPullParser();
        parser.setInput(in, null);

        int eventType = parser.getEventType();
        while (eventType != XmlPullParser.END_DOCUMENT) {
            if (eventType == XmlPullParser.START_DOCUMENT) {
                Log.v(LOG_TAG, "Start of Document");
            } else if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("software_management")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    uris = ParseSoftwareManagementElement(parser);
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

        Log.v(LOG_TAG, "- ParseSoftwareManagementURIs");
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
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("available_updates")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    uris.available_updates = ParseString("available_updates", parser);
                } else if (tag.equals("available_accessories")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    uris.available_accessories = ParseString("available_accessories", parser);
                } else if (tag.equals("map_updates")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("downloads")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    uris.downloads = ParseString("downloads", parser);
                } else if (tag.equals("delete_notification")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("closed_installations")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("software_management")) {
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
        return uris;
    }

    public static ArrayList<SoftwareAssignment> ParseSoftwareAssignments(InputStream in)
            throws XmlPullParserException, IOException {

        Log.v(LOG_TAG, "+ ParseSoftwareAssignments");

        ArrayList<SoftwareAssignment> list = new ArrayList();

        XmlPullParser parser = Xml.newPullParser();
        parser.setInput(in, null);

        int eventType = parser.getEventType();
        while (eventType != XmlPullParser.END_DOCUMENT) {
            if (eventType == XmlPullParser.START_DOCUMENT) {
                Log.v(LOG_TAG, "Start of Document");
            } else if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("available_updates")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    list = ParseAvailableUpdatesElement(parser);
                    Log.w(LOG_TAG, "Parse: " + list.size());
                }
                else if (tag.equals("available_accessories")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    list = ParseAvailableAccessoriesElement(parser);
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

        Log.v(LOG_TAG, "- ParseSoftwareAssignments");

        return list;
    }

    private static ArrayList<SoftwareAssignment> ParseAvailableUpdatesElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || !(parser.getName().equals("available_updates") )) {
            throw new IllegalStateException();
        }

        ArrayList<SoftwareAssignment> list = new ArrayList();

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("this")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("software")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    list.add(ParseSoftwareElement(parser));
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("available_updates")) {
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

        return list;
    }

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
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("software")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    list.add(ParseSoftwareElement(parser));
                } else if (tag.equals("Signature")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("available_accessories")) {
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

        return list;
    }

    private static SoftwareAssignment ParseSoftwareElement(XmlPullParser parser)
            throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals("software"))) {
            throw new IllegalStateException();
        }
        SoftwareAssignment software = new SoftwareAssignment();
        Log.w(LOG_TAG, "ParseSoftwareElement called");

        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                String tag = parser.getName();
                if (tag.equals("id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.id = ParseString(tag, parser);
                } else if (tag.equals("name")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.name = ParseString(tag, parser);
                } else if (tag.equals("icon")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser);
                } else if (tag.equals("short_description")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.shortDescription = ParseString(tag, parser);
                } else if (tag.equals("long_description")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    SkipElement(parser); //Todo: parseLongDescription
                } else if (tag.equals("size")) {
                    Log.v(LOG_TAG, "Skipping: " + tag);
                    software.size = Double.valueOf(ParseString(tag, parser));
                } else if (tag.equals("software_product_id")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.softwareProductId = ParseString(tag, parser);
                } else if (tag.equals("software_product_version")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.softwareProductVersion = ParseString(tag, parser);
                } else if (tag.equals("status")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.status = SoftwareAssignment.stringToStatus(ParseString(tag, parser));
                } else if (tag.equals("installation_order")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.installationOrder = ParseInstallationOrderElement(parser);
                } else if (tag.equals("type")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.type = SoftwareAssignment.stringToType(ParseString(tag, parser));
                } else if (tag.equals("deliverable_type")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.deliverableType = SoftwareAssignment.stringToDeliverableType(ParseString(tag, parser));
                } else if (tag.equals("installation_type")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.installationType = SoftwareAssignment.stringToInstallationType(ParseString(tag, parser));
                } else if (tag.equals("estimated_installation_time")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.estimatedInstallationTime = Integer.valueOf(ParseString(tag, parser));
                } else if (tag.equals("commission_uri")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    software.commissionUri = ParseString(tag, parser);
                } else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("software")) {
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
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.id = ParseString(tag, parser);
                } else if (tag.equals("status")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.status = InstallationOrder.stringToStatus(ParseString(tag, parser));
                } else if (tag.equals("created_by")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.createdBy = ParseString(tag, parser);
                } else if (tag.equals("created")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.created = ParseString(tag, parser);
                } else if (tag.equals("downloads_uri")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.downloadsUri = ParseString(tag, parser);
                } else if (tag.equals("install_notifications_uri")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.installNotificationsUri = ParseString(tag, parser);
                } else if (tag.equals("installation_report_uri")) {
                    Log.v(LOG_TAG, "Parsing: " + tag);
                    installationOrder.installationReportUri = ParseString(tag, parser);
                }  else {
                    Log.d(LOG_TAG, "Skipping unknown tag: " + tag);
                    SkipElement(parser);
                }
            } else if (eventType == XmlPullParser.END_TAG) {
                if (parser.getName().equals("installation_order")) {
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
        return installationOrder;
    }

    /*
     * ###########################################################################################
     * #                                   PARSER FOR DOWNLOADS                                  #
     * ###########################################################################################
     */

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

 /*
     * ###########################################################################################
     * #                                   PARSER FOR INSTALLNOTIFICATIONS                       #
     * ###########################################################################################
     */

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
    /*
     * ###########################################################################################
     * #                                         AUXILIARY                                       #
     * ###########################################################################################
     */

    private static String ParseString(String tag, XmlPullParser parser) throws XmlPullParserException, IOException {
        int eventType = parser.getEventType();
        if ((eventType != XmlPullParser.START_TAG) || (!parser.getName().equals(tag))) {

            throw new IllegalStateException();
        }

        String string = "";
        int depth = 1;
        while (depth != 0) {
            eventType = parser.next();
            if (eventType == XmlPullParser.START_TAG) {
                depth++;
            } else if (eventType == XmlPullParser.END_TAG) {
                depth--;
            } else if (eventType == XmlPullParser.TEXT) {
                string = parser.getText();
            }
        }
        Log.v(LOG_TAG, "String: " + string);
        return string;
    }

    /**
    /* Skips the current attribute or tag in parser
    /* @param parser XmlPullParser object containg the data
    */
    private static void SkipElement(XmlPullParser parser) throws XmlPullParserException, IOException {
        if (parser.getEventType() != XmlPullParser.START_TAG) {
            throw new IllegalStateException();
        }
        int depth = 1;
        while (depth != 0) {
            switch (parser.next()) {
            case XmlPullParser.END_TAG:
                depth--;
                break;
            case XmlPullParser.START_TAG:
                depth++;
                break;
            }
        }
    }

}