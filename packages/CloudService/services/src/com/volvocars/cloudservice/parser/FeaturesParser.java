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

import com.volvocars.cloudservice.*;

/**
* XmlParser for parsing Cloud related responses
* Follows structure found on https://developer.android.com/training/basics/network-ops/xml.html
*/
public final class FeaturesParser extends XmlParser {
    private static final String null_string = null;
    private static final String FEATURES = "features";

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

        parser.require(XmlPullParser.START_TAG, null_string, FEATURES);
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
}