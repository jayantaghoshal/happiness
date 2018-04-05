#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

# This file contains some utilities invoked via command line or by importing this file.
#
# It should NOT be used from VTS, as VTS does not like the __main__ usage

import audioutils
import shutil
import argparse
import os


#audioutils.squash_stereo(fn)


def analyze(args):
    files = {'org': args.file}
    if not args.no_copy:
        dest = os.path.join('/tmp/', os.path.split(args.file)[1])
        shutil.copy(args.file, dest)
        files['org'] = dest
    if args.stereo and args.channel is not None:
        print("Cannot support both --stereo and --channel")
        exit(1)
    elif args.stereo:
        files['mono'] = audioutils.squash_stereo(files['org'])
    elif args.channel is not None:
        files['mono'] = audioutils.extract_channel(files['org'], args.channel)
    else:  # assume file is correct format - or that things will work anyway
        files['mono'] = files['org']

    files['norm_-0.9'] = audioutils.normalize(files['mono'], level=-0.9)
    files['norm_-0.5'] = audioutils.normalize(files['mono'], level=-0.5)
    files['norm_-0.1'] = audioutils.normalize(files['mono'], level=-0.1)

    sounds_09 = audioutils.get_sounds(files['norm_-0.9'])
    sounds_05 = audioutils.get_sounds(files['norm_-0.5'])
    sounds_01 = audioutils.get_sounds(files['norm_-0.1'])

    print("Sounds for norm -0.9:")
    for s in sounds_09:
        print('\t', s)

    print("Sounds for norm -0.5:")
    for s in sounds_05:
        print('\t', s)

    print("Sounds for norm -0.1:")
    for s in sounds_01:
        print('\t', s)




if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Utilities for audio test dev.")
    parser.add_argument('--no-copy', action='store_true')
    parser.add_argument('--stereo', action='store_true', help='Input file is stereo')
    parser.add_argument('--channel', type=int, help='Use only the given channel (ref-ch is 5)')
    parser.add_argument('file', help='The file to operate on')

    parsed_args = parser.parse_args()
    #print(parsed_args.no_copy)
    analyze(**args)

