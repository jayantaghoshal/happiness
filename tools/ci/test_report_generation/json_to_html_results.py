#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import glob
import string

out_dir = os.path.join(os.environ["WORKSPACE"], os.environ["BUILD_NUMBER"])

html_table_style = """ <style>
#result_table {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
    overflow-x:auto;
    border: 1px solid black;

}

#result_table td, #result_table th {
    border: 1px solid #ddd;
    padding: 8px;
    white-space: nowrap;
}

#result_table tr:nth-child(even){background-color: #f2f2f2;}

#result_table tr:hover {background-color: #ddd;}

#result_table th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: left;
    background-color: #4CAF50;
    color: white;
}
</style>"""

# document.getElementById("JSOUT").innerHTML = "hidden_details".concat(row_no);
html_script = """<script>
function display_log_list(row_no) {
    try{
        var hd = document.getElementById("hidden_log_list".concat(row_no));

        if (hd.style.display == "none"){
            hd.style.display = "block";
        }else{
            hd.style.display = "none";
        }
    }
    catch(error){

    }
}

function display_failure_log(row_no) {
    try{
        var hd = document.getElementById("hidden_failure_log".concat(row_no));

        if (hd.style.display == "none"){
            hd.style.display = "block";
        }else{
            hd.style.display = "none";
        }
    }
    catch(error){

    }
}

function display_classname(row_no) {
    try{
    var hd = document.getElementById("hidden_classname".concat(row_no));

    if (hd.style.display == "none"){
        hd.style.display = "block";
    }else{
        hd.style.display = "none";
    }
    }
    catch(error){

    }

}
function display_test_run_details(row_no) {
    try{
        var hd = document.getElementById("hidden_test_run_details".concat(row_no));

        if (hd.style.display == "none"){
            hd.style.display = "block";
        }else{
            hd.style.display = "none";
        }
    }
    catch(error){

    }
}
function display_test_case_details(row_no) {
    try{
        var hd = document.getElementById("hidden_test_case_details".concat(row_no));

        if (hd.style.display == "none"){
            hd.style.display = "block";
        }else{
            hd.style.display = "none";
        }
    }
    catch(error){

    }
}
function toggleChangesTable() {
    var lTable = document.getElementById("changesTable");
    lTable.style.display = (lTable.style.display == "table") ? "none" : "table";
    }
</script>
"""

html_template_start = """<html>
<head>
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/css/bootstrap.min.css'>
<script src='https://ajax.googleapis.com/ajax/libs/jquery/4.0.0-beta.3/jquery.min.js'></script>
<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/js/bootstrap.min.js'></script>
</head>
"""
html_template_end = """</table></body ></html>"""

html_body_start = """<body class="container-fluid"><div style="text-align:center; font-size:160%;">VCC TEST REPORT
<p id="JSOUT"></p><img src="volvo_logo.png" alt="Volvo logo" width="350" height="300">"""

html_table_start = """
<table id="result_table">
  <tr>
    <th>No.</th>
    <th>Test Module</th>
    <th>Test Type</th>
    <th>Result</th>
  </tr>
"""

table_record = string.Template("""
    <tr onclick="display_log_list(${no});display_classname(${no});display_failure_log(${no});display_test_run_details(${no});display_test_case_details(${no});">
    <td>${no}</td>
    <td>${module_name}</td>
    <td>${test_type}</td>
    <td>${result}</td>
  </tr>""")


def parse_tradefed(Html_file: str, json_file: str, table_record_counter: int):

    # Process tradefed results part
    with open(json_file, 'r') as f:
        tradefed_result = json.load(f)
    directory_name = os.path.basename(
        os.path.dirname(os.path.dirname(json_file)))

    for result in tradefed_result["results"]:  # traverse all test inv
        tr = {}
        tr["no"] = table_record_counter + 1
        try:
            tr["module_name"] = result["name"]
        except Exception:
            tr["module_name"] = "No data"
        tr["test_type"] = "Tradefed"

        if result["errors"] == 0 and result["failures"] == 0 and result["tests"] != 0:
            tr["result"] = "Success"
        elif result["errors"] != 0 or result["failures"] != 0:
            tr["result"] = "Failure"
        elif result["tests"] != 0:
            tr["result"] = "Test not started"
        else:
            tr["result"] = "Something went wrong. Check logs."

        tr["logs"] = result["logs"]
        html_log_list = '<ul id="hidden_log_list' + \
            str(tr["no"]) + '" style="display: none;">'
        for log in tr["logs"]:
           html_log_list = html_log_list + '<li><a href = "./' + directory_name + '/tradefed/' + \
               log + '" target="_blank">' + log + '</a></li>'
        html_log_list = html_log_list + '</ul>'
        tr["result"] = tr["result"] + html_log_list

        try:
            html_failure_log = '<p id="hidden_failure_log' + \
                str(tr["no"]) + '" style="display: none; white-space: pre-wrap;"> <b>console out:\n</b>' + \
                result["failure_log"] + '</p>'
            tr["result"] = tr["result"] + html_failure_log
        except Exception:
            pass

        if tr["module_name"] != "No data":

            html_classname = '<p id="hidden_classname' + \
                str(tr["no"]) + '" style="display: none; white-space: pre-wrap;"> <b>Classname:\n</b>' + \
                result["classname"] + '</p>'
            tr["module_name"] = tr["module_name"] + html_classname

        html_test_run_details = '<table id="hidden_test_run_details' + \
            str(tr["no"]) + '" style="display: none; white-space: pre-wrap;"><tr><td><p> <b>Hostname (test runner):\n</b>' + \
            result["hostname"] + '</p></td>'
        html_test_run_details = html_test_run_details + '<td><p> <b>Time taken:\n</b>' + \
            str(result["time"]) + '</p></td>'
        html_test_run_details = html_test_run_details + '<td><p> <b>Timestamp:\n</b>' + \
            result["timestamp"] + '</p></td></tr></table>'

        tr["module_name"] = tr["module_name"] + html_test_run_details

        Html_file.write(table_record.substitute(tr))
        table_record_counter = table_record_counter + 1
    return table_record_counter


def parse_vts(Html_file: str, json_file: str, table_record_counter: int):

    # Process vts results part
    with open(json_file, 'r') as f:
        vts_result = json.load(f)
    directory_name = os.path.basename(
        os.path.dirname(os.path.dirname(json_file)))

    for result in vts_result["results"]:  # traverse all test inv
        tr = {}
        tr["no"] = table_record_counter + 1
        try:
            tr["module_name"] = result["name"]
        except Exception:
            tr["module_name"] = "No data"
        tr["test_type"] = "VTS"

        try:
            if result["done"] == "true" and result["failed"] == 0 and result["modules_done"] == result["modules_total"]:
                tr["result"] = "Success"
            else:
                tr["result"] = "Failure"
        except Exception:
            tr["result"] = "Something went wrong. Check logs."

        tr["logs"] = result["logs"]
        html_log_list = '<ul id="hidden_log_list' + \
            str(tr["no"]) + '" style="display: none;">'
        for log in tr["logs"]:
           html_log_list = html_log_list + '<li><a href = "./' + directory_name + '/vts/' + \
               log + '" target="_blank">' + log + '</a></li>'
        html_log_list = html_log_list + '</ul>'
        tr["result"] = tr["result"] + html_log_list

        html_testcases = '<ul id="hidden_test_case_details' + \
            str(tr["no"]) + '" style="display: none; white-space: pre-wrap; list-style-position: inside;">'
        for testcase in result["testcases"]:
            html_testcases = html_testcases + '<lh><b>Testcase: </b>' + \
                testcase["testcase_name"] + '</lh>'
            html_failure_log = ""
            for test in testcase["tests"]:
                if test["status"] == "pass":
                    html_testcases = html_testcases + '<li>' + \
                        test["name"] + ' - <div style="color: green;display: inline;">' + \
                        test["status"] + '</div></li>'
                else:
                    html_testcases = html_testcases + '<li>' + \
                        test["name"] + ' - <div style="color: red;display: inline;">' + \
                        test["status"] + '</div></li>'
                    html_failure_log = html_failure_log + "<br>" + test["name"] + \
                        ": " + test["failure_log"]

            if html_failure_log != "":
                html_failure_logs = '<p id="hidden_failure_log' + \
                    str(tr["no"]) + '" style="display: none; white-space: pre-wrap;"> <b>console out:\n</b>' + \
                    html_failure_log + '</p>'
                tr["result"] = tr["result"] + html_failure_logs

        html_testcases = html_testcases + '</ul> '
        tr["module_name"] = tr["module_name"] + html_testcases

        html_test_run_details = '<table id="hidden_test_run_details' + \
            str(tr["no"]) + '" style="display: none; white-space: pre-wrap;"><tr><td><p> <b>Hostname (test runner):\n</b>' + \
            result["hostname"] + '</p></td>'
        try:
            html_test_run_details = html_test_run_details + '<td><p> <b>Time taken:\n</b>' + \
                str(result["runtime"]) + '</p></td>'
        except Exception:
            html_test_run_details = html_test_run_details + '<td><p> <b>Time taken:\n</b>' + \
                "no data" + '</p></td>'
        try:
            html_test_run_details = html_test_run_details + '<td><p> <b>ABI:\n</b>' + \
                result["abi"] + '</p></td></tr></table>'
        except Exception:
            html_test_run_details = html_test_run_details + '<td><p> <b>ABI:\n</b>' + \
                "no data" + '</p></td></tr></table>'

        tr["module_name"] = tr["module_name"] + html_test_run_details

        Html_file.write(table_record.substitute(tr))
        table_record_counter = table_record_counter + 1
    return table_record_counter


def changelog_table(Html_file: str):

    with open(os.path.join(out_dir, "changelog.json"), 'r') as f:
        json_data = json.load(f)

    Html_file.write("""
    <div align="right"><a onclick="toggleChangesTable();" href="#"><u>Changes since last run</u></a></div>
    """)
    Html_file.write("""
    <table id=changesTable class="table table-hover" style="display:none;white-space: pre-wrap; list-style-position: inside;">
    <tr style="padding-top: 12px;padding-bottom: 12px;text-align: left;background-color: #4CAF50;color: white;">
        <th>No.</th>
        <th>Changes since last run</th>
        <th>Commit ID</th>

    </tr>
    """)

    try:
        i = 1
        for changes in json_data["changeSet"]["items"]:
                Html_file.write("<tr><td>" + str(i) + "</td><td>" +
                                changes["comment"] + "</td><td>" + changes["commitId"] + "</td></tr>")
                i = i + 1
    except Exception:
        pass  # ignore if there are no changesets

    Html_file.write("</table>")

def main():
    table_record_counter = 0
    Html_file = open(os.path.join(out_dir, "index.html"), "w")
    Html_file.write(html_template_start)
    Html_file.write(html_table_style)
    Html_file.write(html_script)
    Html_file.write(html_body_start)
    changelog_table(Html_file)
    Html_file.write(html_table_start)

    for filename in glob.iglob(out_dir + '/**/*.json', recursive=True):
        json_file = os.path.join(out_dir, filename)
        if(filename.endswith('tradefed_result.json')):
            table_record_counter = parse_tradefed(
                Html_file, json_file, table_record_counter)
        elif(filename.endswith('vts_result.json')):
            table_record_counter = parse_vts(
                Html_file, json_file, table_record_counter)

    Html_file.write(html_template_end)
    Html_file.close()


if __name__ == "__main__":
    main()
