/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.support.test.filters.SmallTest;
import android.support.test.runner.AndroidJUnit4;

import android.util.Log;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.lang.AssertionError;


@SmallTest
@RunWith(AndroidJUnit4.class)
public class XmlSerializerWrapperTest {

    private static final String LOG_TAG = "XmlUtTests";

    @Test
    public void testSerializeInstallationReportEmpty() {
        String expected = "<?xml version='1.0' ?>" +
        "<installation_report " +
        "xsi:schemaLocation=\"http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report installation_report.xsd\" " +
        "xmlns=\"http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" " +
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" +
        "<installation_order_id></installation_order_id>" +
        "<timestamp></timestamp>" +
        "<report_reason>UNKNOWN</report_reason>" +
        "<download_summary>" +
        "<timestamp></timestamp>" +
        "<total_download_time>0</total_download_time>" +
        "<effective_download_time>0</effective_download_time>" +
        "</download_summary>" +
        "<installation_summary>" +
        "<software_id></software_id>" +
        "<timestamp></timestamp>" +
        "<repeat_resets>0</repeat_resets>" +
        "<total_installation_time>0</total_installation_time>" +
        "</installation_summary>" +
        "<ds:Signature>[Signature content omitted]</ds:Signature>" +
        "</installation_report>";

        InstallationReport report = new InstallationReport();

        String actual = XmlSerializerWrapper.serializeInstallationReport(report);

        if(!actual.equals(expected)) {
            for(int i = 0; i < expected.length(); i++) {
                char expected_char = expected.charAt(i);

                if(i < actual.length()) {
                    if(actual.charAt(i) != expected.charAt(i)) {
                        throw new AssertionError("XML Serialization Error at index " + i + ": Expected: " +
                            getClosestTag(expected, i) + " Actual: " + getClosestTag(actual, i));
                    }
                }
            }
            throw new AssertionError("XML Serialization Error: Actual contains unexpected content:" +
                actual.substring(expected.length()));
        }
    }

    @Test
    public void testSerializeInstallationReportPopulated() {
        String expected = "<?xml version='1.0' ?><installation_report xsi:schemaLocation=\"http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report installation_report.xsd\" xmlns=\"http://schemas.volvocars.biz/conncar/foundation_services/software_management/installation_report\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" +
        "<installation_order_id>installationOrderId</installation_order_id>" +
        "<timestamp>timestamp</timestamp>" +
        "<report_reason>OK</report_reason>" +
        "<download_summary>" +
        "<timestamp>download_timestamp</timestamp>" +
        "<total_download_time>2</total_download_time>" +
        "<effective_download_time>1</effective_download_time>" +
        "<data_file>" +
        "<identifier>datafile_identifier</identifier>" +
        "<target_storage_id>LOCAL_STORAGE_BOOT</target_storage_id>" +
        "<status>FAILED</status>" +
        "</data_file>" +
        "</download_summary>" +
        "<installation_summary>" +
        "<software_id>softwareId</software_id>" +
        "<timestamp>installation_timestamp</timestamp>" +
        "<repeat_resets>0</repeat_resets>" +
        "<total_installation_time>2</total_installation_time>" +
        "<ecu>" +
        "<ecu_address>1001</ecu_address>" +
        "<ecu_retries>2</ecu_retries>" +
        "<ecu_status>VERIFIED</ecu_status>" +
        "<software_part>" +
        "<identifier>software_identifier</identifier>" +
        "<retries>3</retries>" +
        "<measured_installation_time>5</measured_installation_time>" +
        "<status>INSTALLED</status>" +
        "</software_part>" +
        "</ecu>" +
        "</installation_summary>" +
        "<ds:Signature>[Signature content omitted]</ds:Signature>" +
        "</installation_report>";

        InstallationReport report = new InstallationReport();

        report.installationOrderId = "installationOrderId";
        report.timestamp = "timestamp";
        report.reportReason = InstallationReport.ReportReason.OK;

        DownloadSummary download = new DownloadSummary();

        download.timestamp = "download_timestamp";
        download.totalDownloadTime = 2;
        download.effectiveDownloadTime = 1;

        DataFile dataFile = new DataFile();

        dataFile.identifier = "datafile_identifier";
        dataFile.targetStorageId = DataFile.TargetStorageId.LOCAL_STORAGE_BOOT;
        dataFile.status = DataFile.Status.FAILED;

        download.dataFiles.add(dataFile);

        report.downloadSummary = download;

        InstallationSummary installation = new InstallationSummary();

        installation.softwareId = "softwareId";
        installation.timestamp = "installation_timestamp";
        installation.repeatResets = 0;
        installation.totalInstallationTime = 2;

        Ecu ecu = new Ecu();

        ecu.address = 1001;
        ecu.retries = 2;
        ecu.status = Ecu.Status.VERIFIED;

        SoftwarePart software = new SoftwarePart();

        software.identifier = "software_identifier";
        software.retries = 3;
        software.measuredInstallationTime = 5;
        software.status = SoftwarePart.Status.INSTALLED;

        ecu.softwareParts.add(software);

        installation.ecus.add(ecu);

        report.installationSummary = installation;

        String actual = XmlSerializerWrapper.serializeInstallationReport(report);

        if(!actual.equals(expected)) {
            for(int i = 0; i < expected.length(); i++) {
                char expected_char = expected.charAt(i);

                if(i < actual.length()) {
                    if(actual.charAt(i) != expected.charAt(i)) {
                        throw new AssertionError("XML Serialization Error at index " + i + ": Expected: " +
                            getClosestTag(expected, i) + " Actual: " + getClosestTag(actual, i));
                    }
                }
            }
            throw new AssertionError("XML Serialization Error: Actual contains unexpected content:" +
                actual.substring(expected.length()));
        }
    }

    private String getClosestTag(String data, int index) {
        String first = data.substring(0, index);
        String second = data.substring(index);

        int startTagIndex = first.lastIndexOf("<");
        int endTagIndex = index + second.indexOf(">");

        return data.substring(startTagIndex, endTagIndex + 1);
    }

}