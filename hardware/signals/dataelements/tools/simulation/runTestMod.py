import importlib
import os
import sys
import time

import tkinter
import tkinter.ttk
import traceback


from Sym import CarSim


def main():
    sys.path.append(os.getcwd())


    root = tkinter.Tk()
    root.style = tkinter.ttk.Style()
    if 'clam' in root.style.theme_names():
        root.style.theme_use("clam")

    root.geometry('1400x1200')

    app = CarSim.App(root)


    modules_to_init = sys.argv[1:]
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
            print(("Failed to load module: %s because: %r" % (p, e) ))
            traceback.print_exc()


if __name__ == "__main__":
    main()
