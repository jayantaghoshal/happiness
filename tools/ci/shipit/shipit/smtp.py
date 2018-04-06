# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import smtplib

from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from typing import Set

FROM = 'no-reply.icup_ci@volvocars.com'


def send_email(to_adress: Set[str], subject: str, html: str):
    msg = MIMEMultipart()
    msg['From'] = FROM
    msg['To'] = ", ".join(to_adress)
    msg['Subject'] = subject

    p1 = MIMEText(html, "html")
    msg.attach(p1)

    with smtplib.SMTP('mailrelay.volvocars.net') as mailserver:
        mailserver.sendmail(FROM, to_adress, msg.as_string())
