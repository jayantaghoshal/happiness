FROM python:3.6.4-stretch

ENV http_proxy 'http://10.244.0.55:83/'
ENV HTTP_PROXY 'http://10.244.0.55:83/'
ENV https_proxy 'https://10.244.0.55:83/'
ENV HTTPS_PROXY 'https://10.244.0.55:83/'
ENV NO_PROXY '.got.volvocars.net'

RUN pip3 install \
    gerritlib \
    requests \
    python-jenkins \
    redis

ARG HOST_UID
ARG HOST_USER

RUN useradd -u ${HOST_UID} -ms /bin/bash ${HOST_USER}
USER ${HOST_USER}
WORKDIR /app

COPY gerrit_event_stream_listener.py .
ENV LOG_DIR /tmp
ENTRYPOINT ["/bin/sh", "-c", "/app/gerrit_event_stream_listener.py >> ${LOG_DIR}/gerrit_event_stream_listener.log 2>&1" ]
