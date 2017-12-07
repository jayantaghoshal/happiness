# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os

AOSP_ROOT = os.path.join(os.path.dirname(__file__), "../../../../../../")

def manifest(aosp_root=AOSP_ROOT):
    return os.path.join(aosp_root, ".repo/manifests")

def volvocars(aosp_root=AOSP_ROOT):
    return os.path.join(aosp_root, "vendor/volvocars")

