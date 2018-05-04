#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

class NoMatch(Exception):
    pass


class Match(object):
    def __init__(self, sound_duration, sound_delta, silence_duration, silence_delta):
        self.sound_duration = sound_duration
        self.sound_delta = sound_delta
        self.silence_duration = silence_duration
        self.silence_delta = silence_delta

    def __str__(self):
        return "Sound-dur: %f, Sound-del: %f, Sil-dur: %f, Sil-del: %f" % (self.sound_duration, self.sound_delta, self.silence_duration, self.silence_delta)

    def __repr__(self):
        return self.__str__()


def _create_combinations(input_list):
    all_lists = list()
    for i in range(len(input_list)):
        all_lists.append(input_list[i:] + input_list[:i])

    return all_lists

def _add_to_log_buffer(buffer, text, indent=0):
    if buffer is None:
        return None
    else:
        buffer.append('.'*4*indent + text)

def _match_sound(sound, match, log_buffer, indent=0):
    _add_to_log_buffer(log_buffer, "Does %s match %s?\n" % (sound, match), indent=indent)
    res = True
    if not (match.sound_duration - match.sound_delta) < sound.duration_sound < (match.sound_duration + match.sound_delta):
        res = False
    if not (match.silence_duration - match.silence_delta) < sound.duration_silence < (match.silence_duration + match.silence_delta):
        res = False
    _add_to_log_buffer(log_buffer, "Matching was %s" % res, indent=indent+1)
    return res


# Tests a list of detected sounds against a list of matches. For a single-component sound the match list have only one
# entry. For a two component sound the match list have two entries: [pling, plong]. The default is to accept sound
# lists starting with both pling and plong as correct, but the internal order must be followed. E.g. both
# pling-plong-pling-plong and plong-pling-plong-pling-plong matches pling-plong, but pling-plong-pling-pling does not.
# Note that the order is preserved so for a three part sound part1-part2-part3, part2-part1-part3 would not match.

def match_sound_silence(sounds, match_list, reorder=True, log_buffer_list=None):
    if len(sounds) < len(match_list):
        raise NoMatch("Sounds detected (%d) MUST be more than sounds to match (%d)" % (len(sounds), len(match_list)))
    all_match_lists = _create_combinations(match_list) if reorder else [match_list,]

    found_match = False
    for m in all_match_lists:
        _add_to_log_buffer(log_buffer_list, ''.join(("For match list ", str(m))))
        for i, s in enumerate(sounds):
            _add_to_log_buffer(log_buffer_list, "Matching sound %d:" % i)
            if not _match_sound(s, m[i % len(m)], log_buffer=log_buffer_list, indent=1):
                break
        else:
            found_match = True

    return found_match

if __name__ == "__main__":
    cl = _create_combinations(('abc', 'def', 'ghi'))
    print(cl)
