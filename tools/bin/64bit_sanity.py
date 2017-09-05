#!/usr/bin/env python3
"""64bit check"""
import glob
import sys
import argparse


def check_64_bit_build_tag(dir_path):
    """Checks for LOCAL_MULTILIB in Android.mk files.
    Returns True if check is a success and False on failure."""
    module_start = False
    module_count = 0
    multilib = False
    error = False
    print(dir_path + "/**/Android.mk")
    filelist = glob.iglob(dir_path + "/**/Android.mk", recursive=True)
    for _file in filelist:
        inputfile = open(_file)
        file_content = inputfile.readlines()
        for line in file_content:

            if line == '\n':
                continue

            if line.startswith("#"):
                continue

            if "$(CLEAR_VARS)" in line:
                module_start = True
                module_count = module_count + 1

            if "LOCAL_MULTILIB:=64" in line.replace(" ", "") and module_start:
                multilib = True

            if "$(BUILD_" in line:
                if not multilib:
                    print("64 bit build not enforced in: " + _file)
                    error = True

                multilib = False
                module_start = False

    if not error:
        print("All " + str(module_count) + " modules have 64 bit builds. Check successfull!")
        return True
    else:
        print("Check failed!")
        return False


def main():
    """Main function that excepts directory path and calls the checker"""
    parser = argparse.ArgumentParser()
    parser.add_argument("dir_path", help="""root directory under which all Android.mk
                        files need to be checked""")
    args = parser.parse_args()

    success = check_64_bit_build_tag(args.dir_path)

    if success:
        sys.exit(0)
    else:
        sys.exit(1)



if __name__ == "__main__":
    main()
