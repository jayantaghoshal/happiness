/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

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
public final class XmlParser
{
    private static final String LOG_TAG ="XmlParser";
    private static final String ns = null;

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

        parser.require(XmlPullParser.START_TAG, ns, "features");
        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String name = parser.getName();
            // Starts by looking for the feature tag
            if (name.equals("feature")) {
                features.add(readFeature(parser));
            } else {
                skip(parser);
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
        parser.require(XmlPullParser.START_TAG, ns, "feature");
        String name="";
        Boolean enabled=false;
        Boolean visible=false;
        String uri="";
        ArrayList<String> tags =null;
        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String attributeName = parser.getName();
            if (attributeName.equals("name")) {
                name = readAttributeAsString(parser,"name");
            }
            else if (attributeName.equals("enabled")) {
                enabled = readAttributeAsBool(parser,"enabled");
            }
            else if (attributeName.equals("visible")) {
                visible = readAttributeAsBool(parser,"visible");
            }
            else if (attributeName.equals("uri")) {
                uri = readAttributeAsString(parser,"uri");
            }
            else if (attributeName.equals("tags")) {
                //Todo implement tags
                skip(parser);
            }
            else {
                skip(parser);
            }
        }
        return new Feature(name,enabled,visible,uri,null);
    }
    /**
    /* Read attribut or (tag) as bool
    /* @param parser XmlPullParser object containg the data
    /* @param attr attribute or (tag) to look for.
    /* @return read attribute or (tag)
    */
    private static Boolean readAttributeAsBool(XmlPullParser parser, String attr) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, ns, attr);
        Boolean name = readAsBool(parser);
        parser.require(XmlPullParser.END_TAG, ns, attr);
        return name;
    }
    /** Read attribute or (tag) as string
    /* @param parser XmlPullParser object containg the data
    /* @param attr attribute or (tag) to look for.
    /* @return read attribute or (tag)
    */
    private static String readAttributeAsString(XmlPullParser parser, String attr) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, ns, attr);
        String name = readAsString(parser);
        parser.require(XmlPullParser.END_TAG, ns, attr);
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
        if(result.toLowerCase().equals("true"))
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
    /**
    /* Skips the current attribute or tag in parser
    /* @param parser XmlPullParser object containg the data
    */
    private static void skip(XmlPullParser parser) throws XmlPullParserException, IOException {
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


    /*
     * ###########################################################################################
     * #                            PARSER FOR SOFTWARE ASSIGNMENT                                #
     * ###########################################################################################
     */

    public static ArrayList<SoftwareAssignment> ParseSoftwareAssignments(InputStream in) throws XmlPullParserException, IOException {
        try {
            XmlPullParser parser = Xml.newPullParser();
            //parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
            //parser.setInput(in, null);
            //parser.nextTag();
            //return readFeatures(parser);
        } finally {
            in.close();
        }
        return new ArrayList();
    }
}