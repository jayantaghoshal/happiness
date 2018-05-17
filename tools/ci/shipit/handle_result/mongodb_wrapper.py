# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from pymongo import MongoClient
import os
import logging
logger = logging.getLogger(__name__)


def mongo_client():
    try:
        client = MongoClient(
            "mongodb://jenkins-icup_android:" + os.environ["MONGODB_PASSWORD"] + "@gotsvl1416.got.volvocars.net:27017/admin?authMechanism=SCRAM-SHA-1")
    except Exception:
        logger.error("Connection to mongoDB server failed")
        raise
    db = client['test_results']
    collection = db['records']
    return collection


def insert_data(json_data):
    collection = mongo_client()
    collection.insert(json_data)
