FROM python:3.6.4-stretch

ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'
ENV NO_PROXY '.got.volvocars.net'

RUN pip3 install \
    requests \
    flask \
    python-jenkins \
    redis

EXPOSE 3002

WORKDIR /app
COPY jenkins_notification_listener.py .

ENV FLASK_APP /app/jenkins_notification_listener.py
ENV LOG_DIR /tmp
ENTRYPOINT ["/bin/sh", "-c", "/usr/local/bin/flask run --host=0.0.0.0 --port=3002 >> ${LOG_DIR}/jenkins_notification_server.log 2>&1" ]
