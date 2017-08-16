#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import re
import sys

from PIL import Image
_num_measures = 120
_graph_height = 200
_graph_width = _num_measures*4

def read_in():
    lines = ""
    for line in sys.stdin:
        lines += line;
    return lines

def _drawRect(img, start_x, start_y, end_x, end_y, color):
    pix = img.load()
    for xpos in range(start_x, end_x):
        for ypos in range(start_y, end_y):
            pix[xpos, ypos] = color

def main():
    lines = read_in().rstrip()
    regex = ur"Frame\s([0-9]+)\stime:\s([0-9]+).*"
    matches = re.finditer(regex, lines)
    img = Image.new("RGB", (_graph_width, _graph_height), "white")
    green_color = (160,254,160)
    _drawRect(img, 0, _graph_height-16, _graph_width, _graph_height, green_color)
    red_color = (254,0,0)
    for matchNum, match in enumerate(matches):
        xVal = int(match.group(1))
        yVal = int(match.group(2))
        if xVal < _num_measures:
            _drawRect(img, xVal*4-3, _graph_height-yVal, xVal*4-1, _graph_height, red_color)
        
    img.show()
    img.save('graph_img.png', 'png')

if __name__ == '__main__':
    main()