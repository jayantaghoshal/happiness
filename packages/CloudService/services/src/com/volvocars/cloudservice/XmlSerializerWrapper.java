/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.util.Log;
import android.util.Xml;

import java.io.IOException;
import java.io.StringWriter;

import java.lang.IllegalArgumentException;
import java.lang.IllegalStateException;

import org.xmlpull.v1.XmlSerializer;


public final class XmlSerializerWrapper {
    private static final String LOG_TAG = "XmlSerializer";

    private static void startDocument(XmlSerializer serializer) throws IOException {
        serializer.startDocument("UTF-8", true);
        serializer.setFeature("http://xmlpull.org/v1/doc/features.html#indent-output", true);
    }

    private static void endDocument(XmlSerializer serializer) throws IOException {
        serializer.endDocument();
    }

    private static void writeTextTag(XmlSerializer serializer, String tagName, String textValue) throws IOException {

        serializer.startTag(null, tagName);

        serializer.text(textValue);

        serializer.endTag(null, tagName);
    }

    private static void signDocument(XmlSerializer serializer, String namespace) throws IOException {
        serializer.startTag(namespace, "Signature");

        serializer.text("[Signature content omitted]");

        serializer.endTag(namespace, "Signature");
    }

    /*
     * ########################################################################
     * #                        Installation Report                           #
     * ########################################################################
     */

    private static void writeDataFile(XmlSerializer serializer, DataFile dataFile) throws IOException {
        serializer.startTag(null, "data_file");

        writeTextTag(serializer, "identifier", dataFile.identifier);
        writeTextTag(serializer, "target_storage_id", dataFile.targetStorageId.toString());
        writeTextTag(serializer, "status", dataFile.status.toString());

        serializer.endTag(null, "data_file");
    }

    private static void writeDownloadSummary(XmlSerializer serializer, DownloadSummary downloadSummary) throws IOException {
        serializer.startTag(null, "download_summary");

        writeTextTag(serializer, "timestamp", downloadSummary.timestamp);
        writeTextTag(serializer, "total_download_time", Long.toString(downloadSummary.totalDownloadTime));
        writeTextTag(serializer, "effective_download_time", Long.toString(downloadSummary.effectiveDownloadTime));

        for(DataFile dataFile : downloadSummary.dataFiles) {
            writeDataFile(serializer, dataFile);
        }

        serializer.endTag(null, "download_summary");
    }

    private static void writeSoftwarePart(XmlSerializer serializer, SoftwarePart softwarePart) throws IOException {
        serializer.startTag(null, "software_part");

        writeTextTag(serializer, "identifier", softwarePart.identifier);
        writeTextTag(serializer, "retries", Integer.toString(softwarePart.retries));
        writeTextTag(serializer, "measured_installation_time", Long.toString(softwarePart.measuredInstallationTime));
        writeTextTag(serializer, "status", softwarePart.status.toString());

        serializer.endTag(null, "software_part");
    }

    private static void writeEcu(XmlSerializer serializer, Ecu ecu) throws IOException {
        serializer.startTag(null, "ecu");

        writeTextTag(serializer, "ecu_address", Integer.toString(ecu.address));
        writeTextTag(serializer, "ecu_retries", Integer.toString(ecu.retries));
        writeTextTag(serializer, "ecu_status", ecu.status.toString());

        for(SoftwarePart softwarePart : ecu.softwareParts) {
            writeSoftwarePart(serializer, softwarePart);
        }

        serializer.endTag(null, "ecu");
    }

    private static void writeInstallationSummary(XmlSerializer serializer, InstallationSummary installationSummary) throws IOException {
        serializer.startTag(null, "installation_summary");

        writeTextTag(serializer, "software_id", installationSummary.softwareId);
        writeTextTag(serializer, "timestamp", installationSummary.timestamp);
        writeTextTag(serializer, "repeat_resets", Integer.toString(installationSummary.repeatResets));
        writeTextTag(serializer, "total_installation_time", Long.toString(installationSummary.totalInstallationTime));

        for(Ecu ecu : installationSummary.ecus) {
            writeEcu(serializer, ecu);
        }

        serializer.endTag(null, "installation_summary");
    }

    public static String serializeInstallationReport(InstallationReport report) {
        StringWriter writer = new StringWriter();
        XmlSerializer serializer = Xml.newSerializer();

        try {
            serializer.setOutput(writer);

            //serializer.setFeature("http://xmlpull.org/v1/doc/features.html#indent-output", true);

            serializer.startDocument(null, null);

            String defaultNamespace = "http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report";
            String signatureNamespace = "http://www.w3.org/2000/09/xmldsig#";
            String instanceNamespace = "http://www.w3.org/2001/XMLSchema-instance";
            serializer.setPrefix("", defaultNamespace);
            serializer.setPrefix("ds", signatureNamespace);
            serializer.setPrefix("xsi", instanceNamespace);

            serializer.startTag(defaultNamespace, "installation_report");
            serializer.attribute(instanceNamespace, "schemaLocation", "http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report installation_report.xsd");

            writeTextTag(serializer, "installation_order_id", report.installationOrderId);
            writeTextTag(serializer, "timestamp", report.timestamp);
            writeTextTag(serializer, "report_reason", report.reportReason.toString());

            writeDownloadSummary(serializer, report.downloadSummary);
            writeInstallationSummary(serializer, report.installationSummary);

            signDocument(serializer, signatureNamespace);

            serializer.endTag(defaultNamespace, "installation_report");

            serializer.endDocument();

        } catch (IOException e) {
            Log.w(LOG_TAG, "Serializing of InstallationReport failed, IOException: [" + e.getMessage() + "]");
            return null;
        } catch (IllegalStateException e) {
            Log.w(LOG_TAG, "Serializing of InstallationReport failed, IllegalStateException: [" + e.getMessage() + "]");
            return null;
        } catch (IllegalArgumentException e) {
            Log.w(LOG_TAG, "Serializing of InstallationReport failed, IllegalArgumentException: [" + e.getMessage() + "]");
            return null;
        }

        return writer.toString();
    }

    /*
     * ########################################################################
     * #                        Installation NOTIFICATION                     #
     * ########################################################################
     */

    private static void writeStatus(XmlSerializer serializer, Status status) throws IOException {
        serializer.startTag(null, "status");

        writeTextTag(serializer, "status_code", status.statusCode.toString());
        writeTextTag(serializer, "sub_status_code", status.subStatusCode.toString());
        writeTextTag(serializer, "sub_status_reason", status.subStatusReason.toString());

        serializer.endTag(null, "status");
    }

    public static String serializeInstallNotification(InstallNotification notification) {
        StringWriter writer = new StringWriter();
        XmlSerializer serializer = Xml.newSerializer();

        try {
            serializer.setOutput(writer);

            //serializer.setFeature("http://xmlpull.org/v1/doc/features.html#indent-output", true);

            serializer.startDocument(null, null);

            String defaultNamespace = "http://schemas.volvocars.biz/conncar/foundation_services/software_management/install_notification";
            String signatureNamespace = "http://www.w3.org/2000/09/xmldsig#";
            String instanceNamespace = "http://www.w3.org/2001/XMLSchema-instance";
            serializer.setPrefix("", defaultNamespace);
            serializer.setPrefix("ds", signatureNamespace);
            serializer.setPrefix("xsi", instanceNamespace);
            serializer.startTag(defaultNamespace, "install_notification");
            serializer.attribute(instanceNamespace, "schemaLocation", "http://schemas.volvocars.biz/conncar/foundation_services/software_management/install_notification install_notification.xsd");

            writeTextTag(serializer, "id", notification.softwareId);
            writeTextTag(serializer, "installation_order_id", notification.installationOrderId);
            writeStatus(serializer, notification.notification.status);

            signDocument(serializer, signatureNamespace);

            serializer.endTag(defaultNamespace, "install_notification");

            serializer.endDocument();

        } catch (IOException e) {
            Log.w(LOG_TAG, "Serializing of InstallNotification failed, IOException: [" + e.getMessage() + "]");
            return null;
        } catch (IllegalStateException e) {
            Log.w(LOG_TAG, "Serializing of InstallNotification failed, IllegalStateException: [" + e.getMessage() + "]");
            return null;
        } catch (IllegalArgumentException e) {
            Log.w(LOG_TAG, "Serializing of InstallNotification failed, IllegalArgumentException: [" + e.getMessage() + "]");
            return null;
        }

        return writer.toString();
    }
}