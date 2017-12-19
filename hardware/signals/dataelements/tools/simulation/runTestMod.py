# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import importlib
import os
import sys
import time
import argparse
import logging

import tkinter
import tkinter.ttk
import traceback

from Sym import CarSim


def app_main(modules_to_init, ip, port):
    sys.path.append(os.getcwd())


    root = tkinter.Tk()
    root.style = tkinter.ttk.Style()
    if 'clam' in root.style.theme_names():
        root.style.theme_use("clam")

    root.geometry('1400x1200')

    app = CarSim.App(root, (ip, port))

    if len(modules_to_init) == 0:
        print("No test module specified in arguments, check the testmodules folder for available modules")
    app.load_modules(modules_to_init)

    root.mainloop()
    time.sleep(0.1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Signal simulator for choosen module(s)')
    parser.add_argument('modules', metavar='Module', type=str, nargs='*',
                        help="module wanted in the simulation, e.g. 'climate'")
    parser.add_argument('--ip', default='198.18.34.1',
                        help='ip-address of IHU interface')
    parser.add_argument('--port', default='8080',
                        help='port of IHU interface')
    parser.add_argument('--log', default='INFO',
                        help='one of DEBUG, INFO (default), WARNING, ERROR or CRITICAL')

    args = parser.parse_args()

    log_level = getattr(logging, args.log.upper(), None)
    logging.basicConfig(level=log_level)
    app_main(args.modules, args.ip, args.port)
