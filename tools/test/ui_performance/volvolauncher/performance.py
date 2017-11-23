#! /usr/bin/env python
# -*- coding: utf-8 -*-
import csv
import sys
import time
import re
import StringIO

def is_number(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

reader = csv.reader(sys.stdin)

intended_vsync = 1
frame_completed = 13
total_frame_count = 0
ok_frame_count = 0
for row in csv.reader(iter(sys.stdin.readline, '')):
    if len(row) == 17:
        total_frame_count += 1
        if is_number(row[1]) and int(row[0]) == 0:
            ok_frame_count += 1
            print("Frame {0} time: {1} ms".format(total_frame_count, (int(row[frame_completed])-int(row[intended_vsync]))/1000000))
#            print("Delta: {0}ms {1}ms".format((int(row[1])-firstTimeInNs)/1000000000, (int(row[12])-firstTimeInNs)/1000000000 ))

# 0  Flags
# 1  IntendedVsync
# 2  Vsync
# 3  OldestInputEvent
# 4  NewestInputEvent
# 5  HandleInputStart
# 6  AnimationStart
# 7  PerformTraversalsStart
# 8  DrawStart
# 9  SyncQueued
# 10 SyncStart
# 11 IssueDrawCommandsStart
# 12 SwapBuffers
# 13 FrameCompleted
# 14 DequeueBufferDuration
# 15 QueueBufferDuration

#Flags
#IntendedVsync
#Vsync

