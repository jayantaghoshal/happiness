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
        dest = os.path.join('/tmp/', args.prefix + os.path.split(args.file)[1])
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

    sounds_90 = audioutils.get_sounds(files['norm_-0.9'], silence_threshold='-90dB', skip_noisy_start=False,
                                      skip_sound_at_eof=False)
    sounds_80 = audioutils.get_sounds(files['norm_-0.9'], silence_threshold='-80dB', skip_noisy_start=False,
                                      skip_sound_at_eof=False)
    sounds_70 = audioutils.get_sounds(files['norm_-0.9'], silence_threshold='-70dB', skip_noisy_start=False,
                                      skip_sound_at_eof=False)
    sounds_50 = audioutils.get_sounds(files['norm_-0.9'], silence_threshold='-50dB', skip_noisy_start=False,
                                      skip_sound_at_eof=False)
    sounds_custom = audioutils.get_sounds(files['norm_-0.9'], silence_threshold=args.silence_threshold,
                                          skip_noisy_start=False, skip_sound_at_eof=False)

    print("Sounds for detection threshold -90dB:")
    for s in sounds_90:
        print('\t', s)

    print("Sounds for detection threshold -80dB:")
    for s in sounds_80:
        print('\t', s)

    print("Sounds for detection threshold -70dB:")
    for s in sounds_70:
        print('\t', s)

    print("Sounds for detection threshold -50dB:")
    for s in sounds_50:
        print('\t', s)

    print("Sounds for CUSTOM detection threshold %s:" % args.silence_threshold)
    for s in sounds_custom:
        print('\t', s)



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Utilities for audio test dev.")
    parser.add_argument('--no-copy', action='store_true')
    parser.add_argument('--stereo', action='store_true', help='Input file is stereo')
    parser.add_argument('--channel', type=int, help='Use only the given channel (ref-ch is 5)')
    parser.add_argument('file', help='The file to operate on')
    parser.add_argument('--prefix', default='')
    parser.add_argument('--silence-threshold', default='-75dB')

    parsed_args = parser.parse_args()
    analyze(parsed_args)

