#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import subprocess
import os.path

# Many of the functions in this file depends on the sox executable
# sudo apt update
# sudo apt-get install sox

class AudioUtilException(Exception):
    pass


class Sound(object):

    def __init__(self, start_sound, stop_sound, stop_silence):
        self.start_sound = start_sound
        self.stop_sound = stop_sound
        self.stop_silence = stop_silence

    def __str__(self):
        return "Sound(Start: %f, Stop: %f, Stop-sil: %f (sound-dur: %f, silence-dur: %f))" % \
               (self.start_sound, self.stop_sound, self.stop_silence, self.duration_sound, self.duration_silence)

    def __repr__(self):
        return self.__str__()

    @property
    def duration_sound(self):
        return self.stop_sound - self.start_sound

    @property
    def duration_silence(self):
        return self.stop_silence - self.stop_sound


# The recording done by alsarecord on the ihu is 6-channel. We are only interested in the ref channel. This function
# will extract one of the channels in the recording to a new file.
def extract_channel(audio_file,  # type: str
                    ch_nbr  # type: int
                    ):
    if audio_file.endswith('.wav'):
        ch_file = audio_file[:-4] + '_ch%d' % ch_nbr + audio_file[-4:]
    else:
        ch_file = audio_file + '_ch%d' % ch_nbr

    proc_res = subprocess.check_output(['sox', '-D', '-R', audio_file, ch_file, 'remix', str(ch_nbr)],
                                       universal_newlines=True)

    if os.path.exists(ch_file):
        return ch_file
    else:
        raise AudioUtilException("No channel extracted to file, sox output %s" % proc_res)


# Converts a stereo wav file to a mono wav file
# It can be used to convert the expected audio (which is mostly stereo) to mono so it can be better compared to
# the reference channel that is mono.
def squash_stereo(audio_file,  # type: str
                  ):
    if audio_file.endswith('.wav'):
        mono_file = audio_file[:-4] + '_mono' + audio_file[-4:]
    else:
        mono_file = audio_file + '_mono'

    proc_res = subprocess.check_output(['sox', '-D', '-R', audio_file, mono_file, 'remix', '1-2'],
                                       universal_newlines=True)

    if os.path.exists(mono_file):
        return mono_file
    else:
        raise AudioUtilException("No stereo file created, sox output %s" % proc_res)


# Normalizes an audio file to a new file
def normalize(audio_file,  # type: str
              level=-0.9  # type: float
              ):
    if audio_file.endswith('.wav'):
        norm_file = audio_file[:-4] + '_norm%f' % abs(level) + audio_file[-4:]
    else:
        norm_file = audio_file + '_norm%f' % abs(level)

    proc_res = subprocess.check_output(['sox', '-D', '-R', audio_file, norm_file, 'norm', str(level)],
                                       universal_newlines=True)

    if os.path.exists(norm_file):
        return norm_file
    else:
        raise AudioUtilException("No normalized file created, sox output %s" % proc_res)

# Splits and audio into segments with sound
def split(audio_file,  # type: str
          sound_list):

    def split_segment(fn,  # type: str
                      seg_nbr,  # type: int
                      start,  # type: int
                      duration  # type: int
                      ):
        if fn.endswith('.wav'):
            seg_file = fn[:-4] + '_part%03d' % seg_nbr + fn[-4:]
        else:
            seg_file = fn + '_part%03d' % seg_nbr

        proc_res = subprocess.check_output(['sox', '-D', '-R', fn, seg_file, 'trim', str(start), str(duration)],
                                           universal_newlines=True)

        if os.path.exists(seg_file):
            return seg_file
        else:
            raise AudioUtilException("No segment file created, sox output %s" % proc_res)

    for i, s in enumerate(sound_list):
        split_segment(audio_file, i, s.start_sound, s.duration_sound)



# This returns a list of Sounds objects that have duration of the sound and duration of the following silence.
# This means that the last sound in the audio recording will NOT be present in this list, as it is not possible
# determine how long the following silence is.
# Also if recording starts noisy, then it is assumed that this is an incomplete sound, and it is skipped.
#
# This function depends on the aubioquiet executable:
# sudo apt-get install aubio-tools libaubio-dev libaubio-doc
#
def get_sounds(audio_file,  # type: str
               silence_threshold='-90dB',  # type: str  # '-90' also ok
               skip_noisy_start=True,  # type: bool
               # skip_sound_at_eof: Only return sounds followed by silence AND start of new sound.
               # Avoids incorrect timing when recording is cut off.
               skip_sound_at_eof=True,  # type: bool
               ):
    res = subprocess.check_output(['aubioquiet', '-s', silence_threshold, audio_file],
#    res = subprocess.check_output(['aubioquiet', audio_file],
                                       universal_newlines=True)

    line_list = res.strip('\n').split('\n')
    # create list with (type, timestamp) tuple
    try:
        split_list = [(part[0], float(part[1])) for part in [line.split(':') for line in line_list]]
    except IndexError:
        print(line_list)
        raise
    sound_list = list()
    if skip_noisy_start and split_list[0][0] == 'NOISY' and split_list[0][1] == 0.0:
        split_list = split_list[1:]
    for i, l in enumerate(split_list):
        if l[0] == 'QUIET':
            continue
        # Index i is now NOISY - we need that and one more QUIET entry to create a sound
        if len(split_list) > i + 2:
            lq = split_list[i+1][1]   # This is the quiet detection following sound
            ln2 = split_list[i+2][1]  # And this is the next sound, so we can know duration of silence
        elif not skip_sound_at_eof:
            res_soxi_len = subprocess.check_output(['soxi', '-D', audio_file],
                                          universal_newlines=True)
            wav_lenght = float(res_soxi_len)
            if len(split_list) == i + 2:
                lq = split_list[i+1][1]  # This is the quiet detection following sound
                ln2 = wav_lenght  # Not the next sound, but best we can do without a next sound
            elif len(split_list) == i + 1:
                lq = wav_lenght  # Assuming end-of-file means end of silence
                ln2 = wav_lenght  # Not the next sound, but best we can do without a next sound
        else:  # skip_sound_at_eof==True
            break

        # If we get here, we know we have a sound followed by a 'complete' quiet
        sound_list.append(Sound(l[1], lq, ln2))
    return sound_list
