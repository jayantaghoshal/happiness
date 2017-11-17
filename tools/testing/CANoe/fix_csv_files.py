import glob
import os
import shutil

# CWD should be in SimulationDB directory when running this script
template = os.path.join(os.path.dirname(__file__), "siglistdirections.csv")
for f in glob.glob("*.vsysvar"):
    basename = f.split(".")[0]
    new_name = basename + ".csv"
    if new_name == template:
        continue
    shutil.copy(template, new_name)