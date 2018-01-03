# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
    This file is used by youcompleteme to provide auto completion for C and C++
    It must be located in any parent directory of the source file you need completion for or configured
    explicitly with g:ycm_global_ycm_extra_conf.

    For more info see https://c1.confluence.cm.volvocars.biz/display/IHUA/IDE
"""

import os
import os.path
import re
import ycm_core


AOSP_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../"))
VCC_ROOT = os.path.join(AOSP_ROOT, "vendor", "volvocars")
#DATABASE = ycm_core.CompilationDatabase(VCC_ROOT)
DATABASE = ycm_core.CompilationDatabase(AOSP_ROOT)

# Hack to get auto completion in header files.
# Because the compile_commands.json only contains source files it does not know which paths to use in header files.
# A reasonable assumption is that you reached the header file by ctrl-clicking it from a source file and then want
# it to use the same include-paths and arguments as the first source file
previous_file = None

def isheader(filename):
    return filename.endswith(".h") or filename.endswith(".hpp") or filename.endswith(".hxx")

def FlagsForFile(filename):
    global previous_file
    file_to_get_db_for = filename

    if isheader(filename):
        if previous_file is not None:
            file_to_get_db_for = previous_file
        else:
            # assume source file in same dir
            sibling_source_file = re.sub(r"(_p)?(\.h|\.hpp|\.hxx)$", ".cpp", filename)
            if os.path.isfile(sibling_source_file):
                file_to_get_db_for = sibling_source_file
    else:
        previous_file = filename

    compilation_info = DATABASE.GetCompilationInfoForFile(file_to_get_db_for)
    final_flags = list(compilation_info.compiler_flags_)

    return {
        'flags': final_flags,
        'include_paths_relative_to_dir': compilation_info.compiler_working_dir_
    }