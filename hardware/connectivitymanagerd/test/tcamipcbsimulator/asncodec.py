#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys

sys.path.append('/usr/local/lib/python2.7/dist-packages')
import asn1tools  # type: ignore


class AsnHelper(object):

    def __init__(self):
        dirpath = os.environ.get('ANDROID_BUILD_TOP')
        asnfilepath = dirpath + \
            '/vendor/volvocars/hardware/infotainmentIpBus/spec/infotainmentIpBus.asn'
        # Compile the ASN.1 specification with Unaligned PER codec
        self.asnlib = asn1tools.compile_files(asnfilepath, 'uper')
