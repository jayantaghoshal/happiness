# What it is
    Test envrionment that allows for buliding simulation modules (for DBUS)
    in the event based way.

# Prerequisites

    NOTE: Do NOT install the prerequisites or use the application in "sourced SDK" terminal
          CarSim is a pure host application

    1. Install prerequisites:
        sudo apt-get install libdbus-1-dev libdbus-glib-1-dev
    2. Install pythons package manager, pip:
        sudo apt install python-pip
    3. Using pip, install Python bindings for libdbus:
        pip install dbus-python
    4. Install Tkinter (python GUI library):
        sudo apt install python-tk

# Running test application

    python runTestMod.py

## Running specific test modules:
    Check available modules in the testmodules folder, include the file name as argument to runTestMod

    ex:

        python runTestMod.py climate
