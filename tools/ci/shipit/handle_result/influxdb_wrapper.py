# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from influxdb import InfluxDBClient
import os
import logging
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

def insert_influx_data(measurement: str, data: dict, tags: dict, timestamp: str = ""):
    row = []
    print(timestamp)
    if not timestamp:
        timestamp = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')
    pointValues = {
        "time": timestamp,
        "measurement": measurement,
        'fields': data,
        'tags': tags,
    }

    row.append(pointValues)
    influxdb = influx_client()
    influxdb.write_points(row)

def query_influx_data(fieldPass: str, fieldFail: str, fieldTotal: str, fieldTime: str, measurement: str, build_number: str):
    influxdb = influx_client()
    results = influxdb.query(("SELECT %s,%s,%s,%s from %s where %s") % (fieldPass, fieldFail, fieldTotal, fieldTime, measurement, build_number))
    return results
