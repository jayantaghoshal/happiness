#! /usr/bin/env python
# -*- coding: utf-8 -*-
import re
import sys
import os
import time
import threading

from com.dtmilano.android.viewclient import ViewClient
from vcc_utilities import *

_screenWidth = 768
_enable_mem = True
_enable_cpu = True
_enable_cpu_proc = True
_enable_gpu = True

def _swipe_with_direction(direction, client):
    if direction == "left":
        client.swipe(0, 100, _screenWidth, 100, 1000)
    else:
        client.swipe(_screenWidth-1, 100, 0, 100, 1000)

_anim_delay = 3 # number of seconds to wait for animations
_seconds_to_measure_cpu = 5 # number of iterations and seconds to fetch cpu info

# Connect to first device detected
kwargs1 = {'ignoreversioncheck': False, 'verbose': False, 'ignoresecuredevice': False}
device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True, 'autodump': False, 'startviewserver': True, 'compresseddump': True}
vc = ViewClient(device, serialno, **kwargs2)

# Press home button first to use as a starting point
pressHomeButton()
time.sleep(_anim_delay)

# Measure ram usage before test start and then again at the end
if _enable_mem:
    make_mem_performance_measurement(1)
    make_mem_total_measurement()

if _enable_cpu_proc:
    make_cpu_performance_measurement_per_process(1)

# Measure CPU for the given amount of seconds, make separate thread in order to be able
# to do things in the UI while the measurement is ongoing
if _enable_cpu:
    cpu_measure_thread = threading.Thread(target=make_cpu_performance_measurement, args=[_seconds_to_measure_cpu], kwargs={})
    cpu_measure_thread.start()

# Do some swipes back and forward and then measure framerate
print("---START UI TEST---")
_swipe_with_direction("left", vc)
_swipe_with_direction("right", vc)
_swipe_with_direction("right", vc)
_swipe_with_direction("left", vc)
_swipe_with_direction("left", vc)
_swipe_with_direction("right", vc)
_swipe_with_direction("left", vc)
_swipe_with_direction("right", vc)
print("---END UI TEST---")

if _enable_gpu:
    make_graphics_performance_measurement(1)

# Synchronise the cpu monitor thread, will block until the thread is done
if _enable_cpu:
    cpu_measure_thread.join()

# Make final mem measurement
if _enable_mem:
    make_mem_performance_measurement(2)

# Make final cpu measurment per process
if _enable_cpu_proc:
    make_cpu_performance_measurement_per_process(2)
