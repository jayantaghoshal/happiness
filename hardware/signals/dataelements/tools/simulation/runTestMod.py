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

def app_main(arg_list, ip, port):
    sys.path.append(os.getcwd())


    root = tkinter.Tk()
    root.style = tkinter.ttk.Style()
    if 'clam' in root.style.theme_names():
        root.style.theme_use("clam")

    root.geometry('1400x1200')

    app = CarSim.App(root, (ip, port))


    modules_to_init = arg_list
    if len(modules_to_init) == 0:
        print("No test module specified in arguments, check the testmodules folder for available modules")
    load_modules(app, modules_to_init)


    root.mainloop()
    time.sleep(0.1)


def load_modules(app, modules):
    all_files = os.listdir("testmodules")
    py_files = [file for file in all_files if file.endswith(".py")]
    py_modules = [p[:-3] for p in py_files]

    for p in py_modules:
        if p not in modules:
            continue
        if p == "__init__":
            continue

        try:
            imported_module = importlib.import_module("testmodules." + p)
            imported_module.init(app)
        except Exception as e:
            logging.error("Failed to load module: %s because: %r" % (p, e) )
            traceback.print_exc()


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
