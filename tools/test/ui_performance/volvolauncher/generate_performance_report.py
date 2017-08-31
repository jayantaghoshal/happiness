#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
import sys

from PIL import Image
_num_measures = 120
_graph_height = 200
_graph_width = _num_measures*4
_result_folder = "html"

def read_in():
    lines = ""
    inFile = sys.argv[1]
    with open(inFile,'r') as inputFile:
        for line in inputFile.readlines():
            lines += line;
    return lines

def _drawRect(img, start_x, start_y, end_x, end_y, color):
    pix = img.load()
    for xpos in range(start_x, end_x):
        for ypos in range(start_y, end_y):
            yloc = ypos
            if yloc < 0:
                yloc = 0
            pix[xpos, yloc] = color

def _generateFpsGraph(lines):
    regex = ur"Frame\s([0-9]+)\stime:\s([0-9]+).*"
    matches = re.finditer(regex, lines)
    img = Image.new("RGB", (_graph_width, _graph_height), "white")
    green_color = (160,254,160)
    _drawRect(img, 0, _graph_height-16*4, _graph_width, _graph_height, green_color)
    red_color = (254,0,0)
    for matchNum, match in enumerate(matches):
        xVal = int(match.group(1))
        yVal = int(match.group(2))
        if xVal < _num_measures:
            _drawRect(img, xVal*4-3, _graph_height-yVal*4, xVal*4-1, _graph_height, red_color)

    img.save(_result_folder + '/frame_performance_img.png', 'png')

def _extract_frame_timing(lines):
    toReturn = "<table><tr><td width=50 align=left>Frame</td><td align=right width=50>Time</td></tr>\n"
    regex = ur"Frame\s([0-9]+)\stime:\s([0-9]+).*"
    matches = re.finditer(regex, lines)
    for matchNum, match in enumerate(matches):
        xVal = int(match.group(1))
        yVal = int(match.group(2))
        if xVal < _num_measures:
            toReturn = toReturn + "<tr><td>" + match.group(1) + "</td><td>" + match.group(2) + "ms</td></tr>\n"
    toReturn = toReturn + "</table>\n"
    return toReturn;

def _extract_cpu_usage(lines):
    toReturn = ""
    regex = ur"---START.CPU.MEASURE\s([0-9]+)---\n((?:.*\n)*?)---END.CPU.MEASURE.[0-9]+---"
    matches = re.finditer(regex, lines)
    toReturn += "<table>\n"
    firstRound = 1
    system = [0,0,0,0,0,0,0,0,0]
    user = [0,0,0,0,0,0,0,0,0]
    nice = [0,0,0,0,0,0,0,0,0]
    idle = [0,0,0,0,0,0,0,0,0]
    for matchNum, match in enumerate(matches):
        cpu_regex = ur"cpu(\d)*\s*(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)"
        cpu_matches = re.finditer(cpu_regex, match.group(2))
        sample_number = int(match.group(1)) - 1
        if sample_number > 0:
            toReturn += "<tr><td colspan=3><b>Sample: " + str(sample_number) + "</b></td></tr>\n"
            toReturn += "<tr><td>CPU</td><td>User</td><td>Nice</td><td>System</td><td>Idle</td></tr>\n"

        for matchNumInt, matchInt in enumerate(cpu_matches):
            if matchInt.group(1) is None:
                index = 0
            else:
                index = int(matchInt.group(1))+1
            if firstRound == 0:
                toReturn += "<tr>\n"
                if index == 0:
                    toReturn += "<td>All</td>\n"
                else:
                    toReturn += "<td>" + str(index) + "</td>\n"
                total = float(matchInt.group(2))-user[index] + int(matchInt.group(3)) - nice[index] + int(matchInt.group(4)) - system[index] + int(matchInt.group(5)) - idle[index]
                diff1 = float(matchInt.group(2))-user[index]
                diff2 = float(matchInt.group(3))-nice[index]
                diff3 = float(matchInt.group(4))-system[index]
                diff4 = float(matchInt.group(5))-idle[index]
                toReturn += "<td>" + str(100*round(diff1/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff2/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff3/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff4/total,3)) + "%</td>\n"
                toReturn += "</tr>\n"
            user[index] = int(matchInt.group(2))
            nice[index] = int(matchInt.group(3))
            system[index] = int(matchInt.group(4))
            idle[index] = int(matchInt.group(5))
        firstRound = 0
    toReturn += "</table>\n"
    return toReturn

def _extract_mem_usage(lines):
    toReturn = ""
    regex = ur"---START.MEM.MEASURE\s([0-9]+)---\n((?:.*\n)*?)---END.MEM.MEASURE.[0-9]+---"
    matches = re.finditer(regex, lines)
    toReturn += "<table>\n"
    for matchNum, match in enumerate(matches):
        toReturn += "<tr><td>Measurement:" + match.group(1) + "</td></tr>"
        for line in match.group(2).splitlines():
            toReturn += "<tr>\n"
            for value in line.split(","):
                toReturn += "<td>" + str(value) + "</td>\n"
            toReturn += "</tr>\n"
    toReturn = toReturn + "</table>\n"
    return toReturn

def _generateMainHtml(lines):
    f = open(_result_folder + '/index.html', 'w')
    f.write('<html>\n')
    f.write('<head></head>\n')
    f.write('<body>\n')
    f.write('<H1>Performance measurements</H1>\n')
    f.write('<table><tr valign="top"><td valign="top">\n')
    f.write('<H2>Graphics</H2>\n')
    f.write('Shows valid frames only, green is objective line for 60fps target<br>\n')
    f.write('<img src="frame_performance_img.png" border=1/>\n')
    f.write(_extract_frame_timing(lines))
    f.write('</td><td><H2>CPU</H2>\n')
    f.write('Usage from a number of samples separated with one second<br>\n')
    f.write(_extract_cpu_usage(lines))
    f.write('</td><td><H2>Memory</H2>\n')
    f.write('Memory usage from the relative measuring points<br>\n')
    f.write(_extract_mem_usage(lines))
    f.write('</td></tr></table>\n')
    f.write('</body>\n')
    f.close()

def main():
    if (len(sys.argv) < 2):
        print "usage: " + sys.argv[0] + " <file to analyze>"
        sys.exit(1)
    lines = read_in()
    if not os.path.exists(_result_folder):
        os.mkdir(_result_folder)
    _generateFpsGraph(lines)
    _generateMainHtml(lines)

if __name__ == '__main__':
    main()
