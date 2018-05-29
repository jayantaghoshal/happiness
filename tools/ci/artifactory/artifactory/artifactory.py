#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import requests
import sys
import os
import hashlib
import time
import tempfile
import json

class Artifactory(object):

    def __init__(self, uri='https://swf1.artifactory.cm.volvocars.biz/artifactory', verbose=False):
        super(Artifactory, self).__init__()
        self.uri = uri
        self.verbose=verbose
        try:
            self._verify_uri(self.uri)
        except:
            print("failed for URL: " + self.uri)
            return None
        if self.verbose: print("URI OK: " + self.uri)

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

    @staticmethod
    def meta_create():
        try:
            fp = tempfile.NamedTemporaryFile(prefix='meta_', suffix='.tmp', delete=False)
        except:
            print('failed to create a temporary file')

        _name = fp.name
        fp.close()
        return _name

    @staticmethod
    def meta_add(metafile, key, value):
        '''Add a key, value pair to the META file.'''
        if not os.path.isfile(metafile):
            return False
        _content = {}
        with open(metafile, 'r') as fp:
            try:
                _content = json.load(fp)
            except:
                None

        _content[key] = value

        with open(metafile, 'w') as fp:
            json.dump(_content, fp, ensure_ascii=False)

        return True

    @staticmethod
    def meta_destroy(metafile):
        '''Destroy a META file object and return the path.'''
        try:
            os.remove(metafile)
        except:
            print('failed to delete, ignored')

    def deploy_artifact(self, uri, filename):
        headers = {
            'X-Checksum-Deploy': "false",
            'X-Checksum-Sha1': self._check_sum("sha1", filename),
            'X-Checksum-Sha256': self._check_sum("sha256", filename)
        }

        print("Artifactory - %10s: %s -> %s" % ('deploy', filename, uri))
        response = self._upload(self._url(uri), filename, headers=headers)
        response.raise_for_status()
        return response

    def set_properties(self, uri, metafile):
        '''Set Arifactory properties for a file/folder stored in a file.'''
        headers = {}
        headers.update(self._headers())
        print('Artifactory - %10s: %s, from file: %s' % ('setProps', uri, metafile))
        with open(metafile, 'r') as fp:
            _content = json.load(fp)

        # construct the complex URL and ensure that
        # special characters are escaped
        _str = "?properties="
        for key in _content.keys():
            if self.verbose:
                print('  %30s: %s' % (key, _content[key]))
            _str += '%s=%s;' % (key, self._htmlifystring(_content[key]))
        _str += 'recursive=1'

        url = self._url('api/storage/' + uri + _str)
        if self.verbose:
            print('URL: %s' % (url))
        response = requests.put(url, stream=False, headers=headers)
        response.raise_for_status()
        return response

    @staticmethod
    def _htmlifystring(_str):
        '''Escape certain characters and return the _str'''
        _str = _str.replace(';', '%5C;')
        _str = _str.replace('=', '%5C=')
        _str = _str.replace(',', '%5C,')
        _str = _str.replace('\/', '%5C/')
        return _str

    def properties(self, path):
        url = self._url("api/storage/"+path)
        headers = {}
        headers.update(self._headers())
        response = requests.get(url, headers=headers)
        response.raise_for_status()
        return response

    def retreive_latest_uri(self, path):
        list_of_builds = self.properties(path)
        creation_dates = []
        build_uris = []

        for build in list_of_builds.json()['children']:
            build_uri = path + build['uri']
            list_of_jobs = self.properties(build_uri)
            creation_dates.append(list_of_jobs.json()['created'])
            build_uris.append(build_uri)

        latest_position = creation_dates.index(max(creation_dates))
        return build_uris[latest_position]

    def _verify_uri(self, uri):
        '''Send a get request to verify the URI'''
        r = requests.get(uri)
        r.raise_for_status()

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
        return self.uri

    def _headers(self):
        return {
            'X-JFrog-Art-Api': self._api_key(),
            'User-Agent': 'artifactory.py'
        }

    def _api_key(self):
        if not "ARTIFACTORY_API_KEY" in os.environ:
            sys.exit("Environment variable ARTIFACTORY_API_KEY is missing!")
        return os.environ.get('ARTIFACTORY_API_KEY')
