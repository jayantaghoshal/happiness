# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import re
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import requests

FROM = 'CI Test Reporting <no-reply@volvocars.net>'
out_dir = os.path.join(os.environ["WORKSPACE"], os.environ["BUILD_NUMBER"])
BUILD_NUMBER = os.environ["BUILD_NUMBER"]
JOB_NAME = os.environ["JOB_NAME"]

def send_email(receiver: str, subject: str, html: str):
    msg = MIMEMultipart()
    msg['From'] = FROM
    msg['To'] = receiver
    msg['Subject'] = subject

    p1 = MIMEText(html, "html")
    msg.attach(p1)

    with smtplib.SMTP('mailrelay.volvocars.net') as mailserver:
        mailserver.sendmail(FROM, receiver, msg.as_string())


def send_hourly_failed_email(commiter_name: str, commiter_mail: str, report_link: str):

    html_contents = """
        <h1 align="center">Testing Mail Notification, Please ignore</h1>
        <h1>"""+JOB_NAME+""" build result</h1>
        <br/>
        <h2>Failing tests found.</h2>
        check the test report in the following link:
        <a href=""" + report_link + """ > """ + report_link + """ </a>
        <br/><br/>
        Note: The failure may be caused by your commit.
        """

    subject = JOB_NAME + " failed!!!"
    print(commiter_mail)
    send_email(commiter_mail, subject, html_contents)


def fetch_commiters_name():
    JENKINS_USER = os.environ["JENKINS_USER"]
    JENKINS_PASSWORD = os.environ["JENKINS_PASSWORD"]

    # fetch json data of changeset
    with open(os.path.join(out_dir, "changelog.json"), 'r') as f:
        json_data = json.load(f)

    try:
        autobump_jenkins_urls = []
        # get manifest autobump jobs
        for changes in json_data["changeSet"]["items"]:
                autobump_jenkins_urls.append(
                    re.search("(?P<url>https?://[^\s]+)", changes["comment"]).group("url"))

        commiters_list = []
        if autobump_jenkins_urls:
            for jenkin_url in autobump_jenkins_urls:  # get commiters detail from manifest autobump job
                response = requests.get(
                    jenkin_url + 'api/json?pretty=true', auth=(JENKINS_USER, JENKINS_PASSWORD))
                for items in response.json()["actions"]:
                    for key, value in items.items():
                        if key == 'parameters':
                            for tmp in value:
                                if tmp['name'] == 'GERRIT_CHANGE_OWNER_NAME':
                                    commiter_name = tmp['value']
                                if tmp['name'] == 'GERRIT_CHANGE_OWNER_EMAIL':
                                    commiter_mail = tmp['value']
                commiters_list.append(
                    {"commiter_name": commiter_name, "commiter_mail": commiter_mail})

        return commiters_list
    except Exception:
        pass  # ignore if there are no changesets


def uniqueList(commiters_list):
    unique_commiters = []
    for commiter in commiters_list:
            if commiter not in unique_commiters:
                unique_commiters.append(commiter)
    return unique_commiters

def main():
    commiters_list = fetch_commiters_name()
    # remove repetition due to multiple commits by a person
    commiters_list = uniqueList(commiters_list)
    print("commiters list: ")
    print(commiters_list)
    if JOB_NAME == "ihu_hourly":
        # TODO hourly is frequent in a day so mailing would annoy the developers. Enable when all tests are passing and stable.
        return
        report_link = "http://gotsvl1416.got.volvocars.net/hourly_test_report/" + \
            str(BUILD_NUMBER)
    elif JOB_NAME == "ihu_daily":
        report_link = "http://gotsvl1416.got.volvocars.net/daily_test_report/" + \
            str(BUILD_NUMBER)

    for commiter in commiters_list:
        # TODO send mail notifications to recipients in other domains
        # send mail to only receipts in volvocars domain
        if "volvocars" in commiter["commiter_mail"]:
            send_hourly_failed_email(
                commiter["commiter_name"], commiter["commiter_mail"], report_link)

if __name__ == '__main__':
    main()
