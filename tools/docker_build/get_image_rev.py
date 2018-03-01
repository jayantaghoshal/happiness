# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""

    Parse the docker image tag from a standard image reference file
    in a more reliable way, instead of counting bytes we'll make use
    of a regular expression to solve it.

"""

import re
import sys
import argparse

REGEXP_SHA = re.compile(
    r'^(?P<url>[\w\.]*:[\d]*)(?P<img_path>/.*)I(?P<image_tag>[a-f0-9]{40})$')
PATH_IMAGE_REF = 'tools/docker_build/image.ref'


def main():

    # handle initial arguments first
    parser = argparse.ArgumentParser(
        description='retrieves docker image tag from standard image_ref file.')
    parser.add_argument('-I', '--imageref', required=True)

    # parse incoming arguments
    args = parser.parse_args()

    try:
        f = open(args.imageref, 'r')
    except:
        print("could not find image_ref file")
        sys.exit(1)

    image_ref = f.readline()
    f.close()

    match = REGEXP_SHA.match(image_ref)
    if not match:
        sys.exit(1)

    print(match.group('image_tag'))

    sys.exit(0)


if __name__ == '__main__':
    main()
