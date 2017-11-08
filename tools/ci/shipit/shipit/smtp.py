import smtplib

from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from typing import Set

FROM = 'no-reply@icup_ci'


def send_email(to_list: Set[str], subject: str, html: str):
    msg = MIMEMultipart()
    msg['From'] = FROM
    msg['To'] = ", ".join(to_list)
    msg['Subject'] = subject

    p1 = MIMEText(html, "html")
    msg.attach(p1)

    with smtplib.SMTP('mailrelay.volvocars.net') as mailserver:
        mailserver.sendmail(FROM, ", ".join(to_list), msg.as_string())
