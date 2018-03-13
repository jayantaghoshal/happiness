# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import zipfile
import re
import sys

def extract_files(path):
    found = False
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.zip'):
                if unzip(root, file):
                    found = True
    return found

def unzip(folder,file):
    try:
        print("Unpacking file: {}".format(os.path.join(folder,file)))
        zip_ref = zipfile.ZipFile(os.path.join(folder,file), 'r')
        zip_ref.extractall(folder)
        zip_ref.close()
        return True
    except Exception:
        print("Could not unpack file: " + str(os.path.join(folder,file)))
        return False

def get_fails(path):
    fails = []
    pattern = '<Test result="fail" name="([^"]*)"'
    regex = re.compile(pattern)
    for root, dirs, files in os.walk(path):
        for file in files:
            if not file.endswith('test_result.xml'):
                continue
            with open(os.path.join(root,file)) as f:
                content = f.read()
                for m in re.finditer(pattern, content):
                    fails.append(m.group(1))
    return fails

def compare_fails(new, old):
    fails = []
    for f in new:
        if not f in old:
            fails.append(f)
    return fails

def main():
    if len(sys.argv) < 2:
        print("Need two folders as arguments, exit")
        sys.exit(1)
    new = sys.argv[1]
    old = sys.argv[2]

    if not extract_files(old):
        print("No results found in 'old' folder ({}), aborting comparison".format(old))
        sys.exit(0)
    if not extract_files(new):
        print("No results found in 'new' folder ({}), aborting comparison".format(new))
        sys.exit(0)

    new_fails = compare_fails(get_fails(new), get_fails(old))

    if len(new_fails) == 0:
        print("Could not find any new errors since last run, great!")
        sys.exit(0)
    else:
        print("{} new errors since last run detected. Bad!".format(len(new_fails)))
        print("List of test cases which passed last run:")
        for fail in new_fails:
            print(fail)
        sys.exit(1)

if __name__ == "__main__":
    main()

