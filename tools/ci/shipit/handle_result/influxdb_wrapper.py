# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
from influxdb import InfluxDBClient
from influxdb.client import InfluxDBClientError
import json
import os
import glob
import time
import logging
import logging.config
from datetime import datetime
logger = logging.getLogger(__name__)

INFLUXDBNAME = 'citestoverview'

host='gotsvl1416.got.volvocars.net'
port=8086

def influx_client():
    try:
        client = InfluxDBClient(host, port, os.environ["INFLUXDB_USER"], os.environ["INFLUXDB_PASSWORD"], INFLUXDBNAME)
    except Exception:
        logger.error("Connection to InfluxDB server failed")
        raise
    return client


def insert_influx_data(measurement: str, data: dict, tags: dict):
    row = []
    current_time = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')
    pointValues = {
        "time": current_time,
        "measurement": measurement,
        'fields': data,
        'tags': tags,
    }

    row.append(pointValues)
    influxdb = influx_client()
    influxdb.write_points(row)

