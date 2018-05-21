/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice.parser;

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
public class XmlParser {
    protected static final String LOG_TAG = "CloudService.XmlParser";
    private static final String null_string = null;

    /**
    /* Read attribut or (tag) as bool
    /* @param parser XmlPullParser object containg the data
    /* @param attr attribute or (tag) to look for.
    /* @return read attribute or (tag)
    */
    protected static Boolean readAttributeAsBool(XmlPullParser parser, String attr)
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
    protected static String readAttributeAsString(XmlPullParser parser, String attr)
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
    protected static Boolean readAsBool(XmlPullParser parser) throws IOException, XmlPullParserException {
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
    protected static String readAsString(XmlPullParser parser) throws IOException, XmlPullParserException {
        String result = "";
        if (parser.next() == XmlPullParser.TEXT) {
            result = parser.getText();
            parser.nextTag();
        }
        return result;
    }


    protected static String ParseString(String tag, XmlPullParser parser) throws XmlPullParserException, IOException {
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
    protected static void SkipElement(XmlPullParser parser) throws XmlPullParserException, IOException {
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