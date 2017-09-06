#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
import sys

from PIL import Image
_num_measures = 120
_graph_height = 200
_graph_width = _num_measures*4
_mem_graph_width = _graph_width/4

_result_folder = "html"
_green_color = (160,254,160)
_dark_green_color = (81, 163, 157)
_red_color = (183,105,92)
_yellow_color = (205,187,121)

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
    _drawRect(img, 0, _graph_height-16*4, _graph_width, _graph_height, _yellow_color)
    for matchNum, match in enumerate(matches):
        xVal = int(match.group(1))
        yVal = int(match.group(2))
        if xVal < _num_measures:
            _drawRect(img, xVal*4-3, _graph_height-yVal*4, xVal*4-1, _graph_height, _red_color)

    img.save(_result_folder + '/frame_performance_img.png', 'png')

def _generateCpuGraph(filename, total, user, nice, kernel, idle):
    img = Image.new("RGB", (_graph_width, _graph_height), "white")
    idle_color = _green_color
    user_color = _dark_green_color
    kernel_color = _red_color
    nice_color = _yellow_color
    white_color = (250,254,254)
    width = 50
    x_offset = 20
    y_start = _graph_height
    graph_padding = 1

    for i in [0,1,2,3,4]:
        x_start = x_offset + width*i + x_offset*i
        x_end = x_start + width
    	user_pos = int(_graph_height-(_graph_height*user[i] / total[i]))
        kernel_pos = int(user_pos - (_graph_height*kernel[i] / total[i]))
        nice_pos = int(kernel_pos - (_graph_height*nice[i] / total[i]))
        _drawRect(img, x_start, 0, x_end, y_start, idle_color)
        _drawRect(img, x_start+graph_padding, graph_padding, x_end-graph_padding, y_start-graph_padding, white_color)

        _drawRect(img, x_start+graph_padding, user_pos, x_end-graph_padding, y_start-graph_padding, user_color)
        _drawRect(img, x_start+graph_padding, kernel_pos, x_end-graph_padding, user_pos, kernel_color)
        _drawRect(img, x_start+graph_padding, nice_pos, x_end-graph_padding, kernel_pos, nice_color)

    img.save(_result_folder + '/' + filename, 'png')

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
    imageToReturn = "cpu_graph.png"
    regex = ur"---START.CPU.MEASURE\s([0-9]+)---\n((?:.*\n)*?)---END.CPU.MEASURE.[0-9]+---"
    matches = re.finditer(regex, lines)
    toReturn += "<table>\n"
    firstRound = 1
    system = [0,0,0,0,0,0,0,0,0]
    user = [0,0,0,0,0,0,0,0,0]
    nice = [0,0,0,0,0,0,0,0,0]
    idle = [0,0,0,0,0,0,0,0,0]
    totalDiff = [1.0,1.0,1.0,1.0,1.0]
    userDiff = [0,0,0,0,0]
    niceDiff = [0,0,0,0,0]
    systemDiff = [0,0,0,0,0]
    idleDiff = [0,0,0,0,0]
    for matchNum, match in enumerate(matches):
        cpu_regex = ur"cpu(\d)*\s*(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)\s(\d*)"
        cpu_matches = re.finditer(cpu_regex, match.group(2))
        sample_number = int(match.group(1)) - 1
        if sample_number > 0:
            toReturn += "<tr><td colspan=3><b>Sample: " + str(sample_number) + "</b></td></tr>\n"
            toReturn += "<tr><td>CPU</td><td>User</td><td>Nice</td><td>System</td><td>Idle</td></tr>\n"
        i = 0
        for matchNumInt, matchInt in enumerate(cpu_matches):
            if matchInt.group(1) is None:
                index = 0
            else:
                index = int(matchInt.group(1))+1
            if firstRound < 1:
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
                if firstRound == 0:
                    totalDiff[i] = total
                    userDiff[i] = diff1
                    niceDiff[i] = diff2
                    systemDiff[i] = diff3
                    idleDiff[i] = diff4
                    i+=1

                toReturn += "<td>" + str(100*round(diff1/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff2/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff3/total,3)) + "%</td>\n"
                toReturn += "<td>" + str(100*round(diff4/total,3)) + "%</td>\n"
                toReturn += "</tr>\n"
            user[index] = int(matchInt.group(2))
            nice[index] = int(matchInt.group(3))
            system[index] = int(matchInt.group(4))
            idle[index] = int(matchInt.group(5))

        firstRound -= 1
        if firstRound == -1:
            _generateCpuGraph(imageToReturn, totalDiff, userDiff, niceDiff, systemDiff, idleDiff)
    toReturn += "</table>\n"
    return toReturn, imageToReturn

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

def _extract_total_mem_usage(lines):
    toReturn = ""
    imageReturned = "mem_usage.png"
    img = Image.new("RGB", (_mem_graph_width, _graph_height), "white")
    regex = ur"---START.TOTAL.MEM.MEASURE---\n((?:.*\n)*?)---END.TOTAL.MEM.MEASURE---"
    matches = re.finditer(regex, lines)
    toReturn += "<table>\n"
    totalValue = 0
    freeValue = 0

    #First line is total mem and second is free mem
    for matchNum, match in enumerate(matches):
        linecount = 0
        for line in match.group(1).splitlines():
            splitstring = line.split()
            if linecount == 0:
                totalValue = int(splitstring[1])
            if linecount == 1:
                freeValue = int(splitstring[1])
            linecount+=1
            toReturn += "<tr><td>" + str(line) + "</td></tr>\n"
    toReturn = toReturn + "</table>\n"
    percent_free = float(freeValue)/totalValue
    _drawRect(img, 0, int(_graph_height-percent_free*_graph_height), _mem_graph_width, _graph_height, _red_color)
    _drawRect(img, 0, 0, _mem_graph_width, int(_graph_height*(1.0-percent_free)), _dark_green_color)

    img.save(_result_folder + '/'+imageReturned, 'png')
    return toReturn, imageReturned, totalValue, freeValue

def _generateMainHtml(lines):
    cpu_details, cpu_image = _extract_cpu_usage(lines)
    mem_total, mem_image, total, free = _extract_total_mem_usage(lines)
    mem_details = _extract_mem_usage(lines)

    f = open(_result_folder + '/index.html', 'w')
    f.write('<html>\n')
    f.write('<head><link rel="stylesheet" type="text/css" href="styles.css"></head>\n')
    f.write('<body>\n')
    f.write('<H1>Performance measurements</H1>\n')
    f.write('Each graph is clickable for additional details.\n')
    f.write('<table><tr valign="top"><td valign="top">\n')
    f.write('<H2>Graphics</H2>\n')
    f.write('Shows valid frames only, green is objective line for 60fps target<br>\n')
    f.write('<a href="graphics.html"><img src="frame_performance_img.png" border=1/></a><br>\n')
    f.write('<div class="color-box" style="background-color: #CDBB79;"></div>Target time for one frame render (16ms)<br>\n')
    f.write('<div class="color-box" style="background-color: #B7695C;"></div>Frame rendering time<br>\n')
    f.write('</td><td><H2>CPU</H2>\n')
    f.write('First bar is average for all CPUs the rest is the individual cores<br>\n')
    f.write('<a href="cpu.html"><img src="' + cpu_image + '"></a><br>\n')
    f.write('<div class="color-box" style="background-color: #51A39D; left: 20px;margin-right: 30px;"></div>User<br>\n')
    f.write('<div class="color-box" style="background-color: #CDBB79; left: 20px;margin-right: 30px;"></div>Nice<br>\n')
    f.write('<div class="color-box" style="background-color: #B7695C; left: 20px;margin-right: 30px;"></div>Kernel<br>\n')
    f.write('</td><td><H2>Memory ' + str(int(100.0*(1.0-float(free)/float(total)))) + '% free</H2>\n')
    f.write('Total memory usage, green is free memory<br>\n')
    f.write('<a href="mem.html"><img src=' + mem_image + '></a><br>')
    f.write('<div class="color-box" style="background-color: #51A39D;"></div>Free memory<br>\n')
    f.write('<div class="color-box" style="background-color: #B7695C;"></div>Used memory<br>\n')
    f.write('</td></tr></table>\n')
    f.write('</body>\n')
    f.close()

    css = open(_result_folder + '/styles.css', 'w')
    css.write('.color-box {\n')
    css.write('    width: 20px;\n')
    css.write('    height: 20px;\n')
    css.write('    display: inline-block;\n')
    css.write('    background-color: #ccc;\n')
    css.write('    position: relative;\n')
    css.write('    left: 0px;\n')
    css.write('    top: 5px;\n')
    css.write('    margin-right: 10px;\n')
    css.write('}\n')
    css.close()
    mem_overview = _extract_total_mem_usage(lines)
    frame_f = open(_result_folder + '/graphics.html', 'w')
    frame_f.write(_extract_frame_timing(lines))
    frame_f.close()
    cpu_f = open(_result_folder + '/cpu.html', 'w')
    cpu_f.write(cpu_details)
    cpu_f.close()

    mem_f = open(_result_folder + '/mem.html', 'w')
    mem_f.write('<h2>Total memory consumption summary</h2>')
    mem_f.write(mem_total)
    mem_f.write('<h2>Per process  memory consumption</h2>')
    mem_f.write(mem_details)
    mem_f.close()

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
