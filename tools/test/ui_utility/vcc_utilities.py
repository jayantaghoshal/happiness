#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import subprocess
import time
import math
import csv
import sys
import re
import StringIO

from subprocess import Popen, PIPE
from PIL import Image
from PIL import ImageChops

#define some "constants"
inputPath = "/dev/input/event1"
xPosRegister = "53"
yPosRegister = "54"
screenWidth = 768
screenHeight = 1024

def _sendEvent(param1, param2, param3):
    subprocess.call(["adb", "shell", "sendevent", inputPath, param1, param2, param3])

def _sendKeyEvent(itemToSend):
    subprocess.call(["adb", "shell", "input", "keyevent", str(itemToSend)])

def _touchUp():
    _sendEvent("3", "58", "0")
    _sendEvent("3", "57", "4294967295")
    _sendEvent("0", "0", "0")

def _touchDown(xPos, yPos):
    _sendEvent("3", "57", "0")
    _sendEvent("3", "48", "50")
    _sendEvent("3", "58", "129")
    _moveToPosition(xPos, yPos)

# position is between 0 and 32768, both in x and y
def _moveToPosition(x, y):
    _sendEvent("3", xPosRegister, str(32768*x/screenWidth))
    _sendEvent("3", yPosRegister, str(32768*y/screenHeight))
    _sendEvent("0", "0", "0")

def _float_range(start, stop, step):
     r = start
     while r < stop:
         yield r
         r += step

# Drag in a pattern, function test
# Will touch startx, y and then drag to endx using given y span and number of steps
def dragSineWave(startX = 200, endX = 400, yOffset = 200, ySpan = 100, steps=120):
    _moveToPosition(startX,yOffset)
    _touchDown(startX,yOffset)
    for i in _float_range(0, math.pi*2, math.pi*2/steps):
        _moveToPosition(startX+i*(endX-startX)/(math.pi*2), yOffset+math.sin(i)*ySpan)
    _touchUp()

# Press startX, startY and wait for secondsPress seconds
# Drag to endX, endY as quickly as possible with the number of steps given
def pressAndDrag(startX=0, startY=0, secondsPress=3, endX=200, endY=200, numberSteps=50):
    currentXPos = startX
    currentYPos = startY
    _moveToPosition(currentXPos, currentYPos)
    _touchDown(currentXPos, currentYPos)
    time.sleep(secondsPress)
    xStep = (endX-startX) / numberSteps
    yStep = (endY-startY) / numberSteps
    for x in range(1,numberSteps):
        _moveToPosition(currentXPos, currentYPos)
        currentXPos += xStep
        currentYPos += yStep
    _touchUp()

def pressHomeButton():
    _sendKeyEvent(3)


# Image handling functions, MIT licens from imgcompare:
# https://github.com/datenhahn/imgcompare
def _pixel_diff(image_a, image_b):
    """
    Calculates a black/white image containing all differences between the two input images.
    """
    diff = ImageChops.difference(image_a, image_b)
    return diff.convert('L')

def _total_histogram_diff(pixel_diff):
    """
    Sums up all histogram values of an image. When used with the black/white pixel-diff image
    this gives the difference "score" of an image.
    """
    return sum(i * n for i, n in enumerate(pixel_diff.histogram()))

def _image_diff(image_a, image_b):
    """
    Calculates the total difference "score" of two images.
    """
    return _total_histogram_diff(_pixel_diff(image_a, image_b))

def is_equal(image_a, image_b, tolerance=0.0):
    """
    Equal returns true if image diff is less then tolerance in percent
    """
    return image_diff_percent(image_a, image_b) <= tolerance

def image_diff_percent(image_a, image_b):
    """
    Calculate the difference between two images in percent.
    """
    # first determine difference of input images
    input_images_histogram_diff = _image_diff(image_a, image_b)

    # to get the worst possible difference use a black and a white image
    # of the same size and diff them
    black_reference_image = Image.new('RGB', image_a.size, (0, 0, 0))
    white_reference_image = Image.new('RGB', image_a.size, (255, 255, 255))

    worst_bw_diff = _image_diff(black_reference_image, white_reference_image)

    percentage_histogram_diff = (input_images_histogram_diff / float(worst_bw_diff)) * 100

    return percentage_histogram_diff

def make_graphics_performance_measurement(measurement_number):
    """
    Grabs the last 120 frames and returns timing for the relevant frames
    """
    _parse_graphics_measurement(subprocess.check_output("adb shell dumpsys gfxinfo com.volvocars.launcher framestats",stderr=subprocess.STDOUT,shell=True), measurement_number)

def make_mem_performance_measurement(measurement_number):
    """
    Dumps the current memory usage
    """
    _parse_mem_measurement(subprocess.check_output("adb shell dumpsys meminfo -c", stderr=subprocess.STDOUT,shell=True), measurement_number)

def make_mem_total_measurement():
    """
    Grabs an summary of memory usage
    """
    _parse_total_mem_measurement(subprocess.check_output("adb shell cat /proc/meminfo", stderr=subprocess.STDOUT,shell=True))

def make_cpu_performance_measurement_per_process(measurement_number):
    result = "NA"
    try:
        result = subprocess.check_output("adb shell ls /proc/", stderr=subprocess.STDOUT, shell=True).rstrip()
    except subprocess.CalledProcessError as e:
        result = e.output
    regex = ur"([0-9]+).*"
    lost_pids = 0
    matches = re.finditer(regex, result)
    print("---START CPU PER PROCESS MEASURE {0}---".format(measurement_number))
    print("         CPU    PID           PROCESS")
    for matchNum, match in enumerate(matches):
        try:
            string_to_use = ("adb shell cat proc/{0}/stat".format(match.group(1)))
            stat_for_process = subprocess.check_output(string_to_use, stderr=subprocess.STDOUT, shell=True).rstrip()
            extraction_regex = ur"([0-9]+).\((.*)\)(\s+\S*){11}\s(\d*)\s(\d*)"
            internal_matches = re.finditer(extraction_regex, stat_for_process)
            for internalMatchNum, internalMatch in enumerate(internal_matches):
                print("{:>12}  {:>5}  {:>16}".format(int(internalMatch.group(4))+int(internalMatch.group(5)), internalMatch.group(1), internalMatch.group(2)))
        except subprocess.CalledProcessError as e:
            # the process was terminated stat was called, do nothing
            lost_pids += 1
    print("---END CPU PER PROCESS MEASURE {0}---".format(measurement_number))

def make_cpu_performance_measurement(seconds):
    """
    Grabs continuous data for the number of seconds supplied
    """
    counter = 0
    while counter < seconds:
        counter+=1
        print("---START CPU MEASURE {0}---".format(counter))
        time.sleep(1)
        result = subprocess.check_output("adb shell cat /proc/stat", stderr=subprocess.STDOUT, shell=True).rstrip()
        regex = ur".*(cpu.\s).*"
        matches = re.finditer(regex, result)
        for matchNum, match in enumerate(matches):
            print(match.group())
        time.sleep(1)
        print("---END CPU MEASURE {0}---".format(counter))

def _is_number(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def _parse_mem_measurement(to_parse, measurement_number):
    # ram,1885480,961033,556162
    # Total, free, ?
    # proc,pers,com.android.systemui,3727,55524,e
    # proc, ?, name, pid, mem used, ?
    print("---START MEM MEASURE {0}---".format(measurement_number))
    print(to_parse)
    print("---END MEM MEASURE {0}---".format(measurement_number))

def _parse_total_mem_measurement(to_parse):
    print("---START TOTAL MEM MEASURE---")
    print(to_parse)
    print("---END TOTAL MEM MEASURE---")

def _parse_graphics_measurement(to_parse, measurement_number):
    reader = to_parse.split('\n')
    intended_vsync = 1
    frame_completed = 13
    total_frame_count = 0
    ok_frame_count = 0
    slowest_frame = 0
    fastest_frame = 10000
    total_ok_frame_time = 0
    print("---START GRAPHICS MEASURE {0}---".format(measurement_number))
    for row in reader:
        values = row.split(',')
        if len(values) >= 15:
            total_frame_count += 1
            if _is_number(values[1]) and int(values[0]) == 0:
                ok_frame_count += 1
                current_frame_time = (int(values[frame_completed])-int(values[intended_vsync]))/1000000
                print("Frame {0} time: {1} ms".format(total_frame_count, current_frame_time))
                fastest_frame = min(fastest_frame, current_frame_time)
                slowest_frame = max(slowest_frame, current_frame_time)
                total_ok_frame_time += current_frame_time
    print("AVG: {0}ms MIN: {1}ms MAX: {2}ms".format(total_ok_frame_time/max(1,ok_frame_count), fastest_frame, slowest_frame))
    print("---END GRAPHICS MEASURE {0}---".format(measurement_number))
