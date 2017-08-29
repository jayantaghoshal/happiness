#!/usr/bin/env python
import requests
import sys
import os
import hashlib
import time
import json

class Artifactory(object):

    def retrieve_artifacts(self, uri, dest_dir):
        ''' Download all files in directory '''
        response = self.properties(uri)
        for child in response.json()['children']:
            self.retrieve_artifact(uri + child['uri'], dest_dir)

    def retrieve_artifact(self, uri, dest_dir):
        ''' Download file '''
        filename = dest_dir + '/' + os.path.basename(uri)
        print("Retrieve file from Artifactory - %s -> %s/%s" % (uri, dest_dir, filename))
        response = self._download(self._url(uri), filename)
        response.raise_for_status()

        checksum = response.headers["X-Checksum-Md5"]
        self._verify_checksum(filename, checksum)
        return response

    def deploy_artifact(self, uri, filename):
        headers = {
            'X-Checksum-Deploy': "false",
            'X-Checksum-Sha1': self._check_sum("sha1", filename),
            'X-Checksum-Sha256': self._check_sum("sha256", filename)
        }

        print("Deploy file to Artifactory - %s -> %s" % (filename, uri))
        response = self._upload(self._url(uri), filename, headers=headers)
        response.raise_for_status()
        return response

    def properties(self, path):
        url = self._url("api/storage/"+path)
        headers = {}
        headers.update(self._headers())
        response = requests.get(url, headers=headers)
        response.raise_for_status()
        return response

    def _verify_checksum(self, filename, checksum):
        calculated_checksum = self._check_sum("md5", filename)
        if checksum != calculated_checksum:
            raise Exception("Checksum missmatch - expected %s, got %s", checksum, calculated_checksum)

    def _check_sum(self, algorithm, filename):
        BLOCKSIZE = 65536
        hasher = None
        if algorithm == "md5":
            hasher = hashlib.md5()
        elif algorithm == "sha1":
            hasher = hashlib.sha1()        
        elif algorithm == "sha256":
            hasher = hashlib.sha256()
        else:
            raise Exception("Unsupported hash algorithm - %s", algorithm)

        with open(filename, 'rb') as fd:
            buf = fd.read(BLOCKSIZE)
            while len(buf) > 0:
                hasher.update(buf)
                buf = fd.read(BLOCKSIZE)
        return hasher.hexdigest()


    def _upload(self, url, filename, headers={}):
        headers.update(self._headers())

        with open(filename, 'rb') as fd:
            response = requests.put(url,
                                    stream=True,
                                    headers=headers,
                                    data=fd)
        return response

    def _download(self, url, filename, headers={}):
        headers.update(self._headers())
        with open(filename, 'wb') as fd:
            response = requests.get(url,
                                    headers=headers,
                                    stream=False)

            for chunk in response.iter_content(chunk_size=10*1024*1024):
                fd.write(chunk)
        return response

    def _url(self, uri):
        return self._base_url() + '/' + uri

    def _base_url(self):
        return "https://swf1.artifactory.cm.volvocars.biz/artifactory"

    def _headers(self):
        return {
            'X-JFrog-Art-Api': self._api_key(),
            'User-Agent': 'artifactory.py'
        }

    def _api_key(self):
        if not "ARTIFACTORY_API_KEY" in os.environ:
            sys.exit("Environment variable ARTIFACTORY_API_KEY is missing!")
        return os.environ.get('ARTIFACTORY_API_KEY')

