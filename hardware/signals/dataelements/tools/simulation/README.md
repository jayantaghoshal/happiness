# What it is
    Test envrionment that allows for buliding simulation modules

# Prerequisites

    NOTE: Do NOT install the prerequisites or use the application on target.

    1. Install prerequisites (already part of docker, but missing on host):
        sudo apt-get install python3-tk

# Running test application

    python3 runTestMod.py

## Running specific test modules:
    Check available modules in the testmodules folder, include the file name as argument to runTestMod

    ex:

        python3 runTestMod.py climate
