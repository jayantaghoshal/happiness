FROM mongo

RUN apt-get update && \
    apt-get install -y cron curl

#install jfrog client to upload  backups
RUN curl -fL https://getcli.jfrog.io | sh

ADD cronjobs /etc/cron.d/cronjobs

RUN chmod 0644 /etc/cron.d/cronjobs

RUN /usr/bin/crontab /etc/cron.d/cronjobs

CMD ["/bin/sh","/workspace/start.sh"]
