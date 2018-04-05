# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import alsarecorder
import audioutils
import match
import os

if True:
    ar = alsarecorder.AlsaRecorder()
    with ar.alsa_recorder(2, host_folder='/home/obae1/tmp'):
        # Here goes code that sets the signals or triggers sound in some other way
        pass
    fn = ar.host_fn
else:
    #fn = '/home/obae1/tmp/recording_20180405_164103.wav'
    #fn = '/home/obae1/tmp/recording_20180409_134447.wav'
    fn = '/home/obae1/tmp/recording_20180409_122403.wav'

if os.path.exists(fn):
    print("FILE %s EXISTS" % fn)
    exit(0)
else:
    print("FILE %s NOT FOUND" % fn)
    exit(1)

# Extract ref-channel to separate file
ch5_file = audioutils.extract_channel(fn, 5)

# Normalize file to -0.9dB - makes it more similar to input.
# Note that this is not good if we want to check that the audio is played on the correct level
norm_file = audioutils.normalize(ch5_file)

# Get list of sounds (seprated by quiet)
sounds = audioutils.get_sounds(norm_file)

# Match that list of sounds towards a list of Match objects
res = match.match_sound_silence(sounds, [match.Match(0.0587, 0.02, 0.554, 0.05), match.Match(0.218, 0.02, 0.17, 0.02)], reorder=False)

if res:
    print("PASSED!")
else:
    print("NOT MATCHED - FAILED")

